#include "ChunkRenderer.h"
#include "../../Graphics/OpenGL/Shader/RasterShader.h"
#include "../../Graphics/OpenGL/Shader/ComputeShader.h"
#include "../../Graphics/OpenGL/Buffers/VertexBufferObject.h"
#include "../../Graphics/OpenGL/Buffers/IndexBufferObject.h"
#include "../../Graphics/OpenGL/Buffers/VertexArrayObject.h"
#include "../../Graphics/OpenGL/Buffers/ShaderBufferObject.h"
#include "../../Graphics/OpenGL/Buffers/DrawIndirectBufferObject.h"
#include "../../Graphics/Geometry/BlockGeometry.h"
#include "../WorldTransform.h"
#include "../World.h"
#include "../../Player/Player.h"
#include "../Chunk/Chunk.h"
#include "../../Graphics/OpenGL/Render/Renderer.h"
#include "../Chunk/ChunkRenderComponent.h"
#include "../../Graphics/OpenGL/Camera/Camera.h"
#include <glad/glad.h>
#include "../../Engine/Engine.h"
#include "../../Core//Utils/CompileTimeFiles.h"
#include "../../Graphics/OpenGL/Buffers/MappedTripleIbo.h"
#include "../../Graphics/OpenGL/Buffers/MappedTripleDibo.h"
#include "../../Settings/Settings.h"

ChunkRenderer::ChunkRenderer()
  : chunkOffsetUniform("u_chunkOffset"), cameraMvpUniform("u_cameraMVP"), boundDrawCallId(0), modifyDrawCallId(1)
{
  assertOnRenderThread();
  //shader = new Shader(CompileTimeFiles::GetTextFile("Chunk.vert")->contents, CompileTimeFiles::GetTextFile("Chunk.frag")->contents);
  blockShader = new RasterShader(CompileTimeFiles::GetTextFile("Block.vert"_str)->contents, CompileTimeFiles::GetTextFile("Block.frag"_str)->contents);
  blockMultidrawShader = new RasterShader(CompileTimeFiles::GetTextFile("BlockMultidraw.vert"_str)->contents, CompileTimeFiles::GetTextFile("BlockMultidraw.frag"_str)->contents);

  vao = new VertexArrayObject();
  //vao->Bind();
  vao->SetFormatLayout(BlockQuad::GetQuadsVertexBufferLayout());

  multidrawVao = new VertexArrayObject();
  multidrawVao->SetFormatLayout(BlockQuad::GetQuadsVertexBufferLayout());
  multidrawVao->Bind();

  glEnableVertexArrayAttrib(multidrawVao->GetId(), 4);
  //glVertexArrayAttribBinding(id, i, 0);
  //glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (const void*)0);
  glVertexArrayAttribFormat(multidrawVao->GetId(), 4, 3, GL_FLOAT, GL_FALSE, 0);
  //glVertexAttribDivisor(4, 1);
  glVertexArrayBindingDivisor(multidrawVao->GetId(), 4, 1);

  blocksIbo = new IndexBufferObject();

  constexpr uint32 indices[] = { 
    0, 2, 1,
    0, 3, 2
  };
  constexpr uint32 maxIndices = CHUNK_SIZE * 6 * 6;
  uint32* buffer = new uint32[maxIndices];
  uint32 index = 0;
  for (uint32 i = 0; i < (maxIndices / 6); i++) {
    buffer[index++] = (indices[0] + (i * 4));
    buffer[index++] = (indices[1] + (i * 4));
    buffer[index++] = (indices[2] + (i * 4));
    buffer[index++] = (indices[3] + (i * 4));
    buffer[index++] = (indices[4] + (i * 4));
    buffer[index++] = (indices[5] + (i * 4));
  }
  blocksIbo->BufferData(buffer, maxIndices);
  delete[] buffer;

  vao->BindIndexBufferObject(blocksIbo);

  hugeVbo = new LargeRangedVbo<BlockQuad>();
  const uint64 hugeVboReserveCapacity = std::pow(uint64(GetSettings()->GetRenderDistance()), 3ULL) * CHUNK_SIZE / 2;
  //const uint64 hugeVboReserveCapacity = std::pow(uint64(GetSettings()->GetRenderDistance()), 3ULL) * CHUNK_SIZE * 4;
#ifdef CUBE_DEVELOPMENT
  const double _reserveInGB = double(hugeVboReserveCapacity * sizeof(BlockQuad)) / 1000000000.0;
  cubeLog("Allocating " + String::fromFloat(_reserveInGB, 3) + "GB for large VBO");
#endif
  hugeVbo->Reserve(hugeVboReserveCapacity);

  multidrawChunkOffsets = new VertexBufferObject();
  multidrawCommands = new DrawIndirectBufferObject();
}

void ChunkRenderer::SetShaderChunkOffset(glm::vec3 chunkOffset)
{
  blockShader->SetUniform3f(chunkOffsetUniform, chunkOffset);
}

void ChunkRenderer::SetShaderCameraMVP(const glm::mat4& cameraMVP)
{
  blockShader->SetUniformMat4f(cameraMvpUniform, cameraMVP);
}

void ChunkRenderer::Bind()
{
  blockShader->Bind();
  vao->Bind();
}

void ChunkRenderer::SwapNextBuffers()
{
  boundDrawCallId = (boundDrawCallId + 1) % 2;
  modifyDrawCallId = (modifyDrawCallId + 1) % 2;
  // Dont swap chunk buffers. They only get swapped for remesh.
}

void ChunkRenderer::StoreModifyDrawCallData()
{
  DrawCallData& modifyDrawData = drawCalls[modifyDrawCallId];
  modifyDrawData.playerPos = GetWorld()->GetPlayer()->GetLocation();
  modifyDrawData.cameraMVP = Camera::GetActiveCamera()->GetMvpMatrix();
}

void ChunkRenderer::DrawAllChunksAndPrepareNext(darray<ChunkDrawCall> chunksToDrawNextFrame)
{
  // 8. Draw the chunks
  const DrawCallData& boundDrawData = drawCalls[boundDrawCallId];
  PerformBoundDrawCalls();

  DrawCallData& modifyDrawData = drawCalls[modifyDrawCallId];

  darray<BlockQuad> quads;
  darray<uint32> indices;
  uint32 baseVertex = 0;
  
  
  // 9. Copy the remeshed chunks to their OpenGL buffers.
  CopyRemeshedChunksDataToBuffers();

  

  // 10. Prepare the draw calls for the next frame.
  //frameChunkDrawCalls.Empty();
  //for (const ChunkDrawCall& chunk : chunksToDrawNextFrame) {
  //  frameChunkDrawCalls.Add(chunk);
  //}
  frameChunkDrawCalls = std::move(chunksToDrawNextFrame);

  //GeneratedMultidrawIndirectCommands();

  // 11. Swap buffers
  SwapNextBuffers();
}

void ChunkRenderer::SetRemeshedChunks(const darray<Chunk*>& newRemeshedChunks)
{
  remeshedChunks = newRemeshedChunks;
}

void ChunkRenderer::PerformBoundDrawCalls()
{
  Renderer::Clear();
  vao->Bind();

  if (frameChunkDrawCalls.Size() == 0) {
    engine->SwapGlfwBuffers();
    return;
  }
#if false
  DrawAllFrameChunksMultipleDrawCalls();
#else
  MultidrawAllFrameChunks();
#endif
}

void ChunkRenderer::DrawChunk(const Chunk* drawChunk)
{
  VertexBufferObject* vbo = drawChunk->GetRenderComponent()->GetVbos()->GetBoundBuffer();
  IndexBufferObject* ibo = drawChunk->GetRenderComponent()->GetIbos()->GetBoundBuffer();
  //if (vbo == nullptr || ibo == nullptr) return;
  //if (ibo->GetIndexCount() == 0) return;

  vao->BindVertexBufferObject(vbo, sizeof(BlockVertex));
  vao->BindIndexBufferObject(ibo);
  SetShaderChunkOffset(GetChunkShaderPositionOffset(drawCalls[boundDrawCallId].playerPos, drawChunk));
  glDrawElements(GL_TRIANGLES, ibo->GetIndexCount(), GL_UNSIGNED_INT, 0);
}

void ChunkRenderer::CopyRemeshedChunksDataToBuffers()
{
  for (Chunk* chunk : remeshedChunks) {
    ChunkRenderComponent* renderComponent = chunk->GetRenderComponent();

    ChunkMesh* mesh = renderComponent->GetMesh();
    if (mesh->GetQuadCount() == 0) continue;

    if (!renderComponent->AreGLBuffersInitialized()) {
      renderComponent->CreateGLBuffers();
    }
    if (!renderComponent->HasEnoughVboRangeCapacityForMesh()) {
      renderComponent->ReallocateVboRangeForMesh(this);
    }

    auto cvbos = renderComponent->GetVbos();
    auto cibos = renderComponent->GetIbos();

    if (cvbos->GetCapacity() < mesh->GetQuadCount()) {
      cvbos->Reserve(static_cast<int>(mesh->GetQuadCount() * 1.5));
    }
    if (cibos->GetCapacity() < mesh->GetIndexCount()) {
      cibos->Reserve(static_cast<int>(mesh->GetIndexCount() * 1.5));
    }

    auto& mappedVbo = cvbos->GetModifyMapped();
    memcpy(mappedVbo.data, mesh->GetQuadsData(), mesh->GetQuadCount() * sizeof(BlockQuad));
    auto& mappedIbo = cibos->GetModifyMapped();
    memcpy(mappedIbo.data, mesh->GetIndices().Data(), mesh->GetIndexCount() * sizeof(uint32));
    mappedIbo.buffer->SetIndexCount(mesh->GetIndexCount());

    VboMappedRangeRef<BlockQuad>* vboRange = renderComponent->GetVboRange();
    gk_assertNotNull(vboRange);
    vboRange->WriteToRange(renderComponent->GetMesh()->GetQuadsData(), renderComponent->GetMesh()->GetQuadCount());

    cvbos->SwapNextBuffer();
    cibos->SwapNextBuffer();
  }
  remeshedChunks.Empty();
}

void ChunkRenderer::DrawAllFrameChunksMultipleDrawCalls()
{
  const DrawCallData& boundDrawData = drawCalls[boundDrawCallId];
  blockShader->Bind();
  SetShaderCameraMVP(boundDrawData.cameraMVP);

  vao->BindIndexBufferObject(blocksIbo);
  vao->BindVertexBufferObject(hugeVbo->GetVbo(), sizeof(BlockVertex));

  for (uint32 i = 0; i < frameChunkDrawCalls.Size(); i++) {
    SetShaderChunkOffset(frameChunkDrawCalls[i].chunkPositionOffset);
#if false // should draw using individual buffers
    //DrawChunk(frameChunkDrawCalls[i]);
    VertexBufferObject* vbo = frameChunkDrawCalls[i].chunk->GetRenderComponent()->GetVbos()->GetBoundBuffer();
    //IndexBufferObject* ibo = frameChunkDrawCalls[i].chunk->GetRenderComponent()->GetIbos()->GetBoundBuffer();
    //if (vbo == nullptr || ibo == nullptr) return;
    //if (ibo->GetIndexCount() == 0) return;

    vao->BindVertexBufferObject(vbo, sizeof(BlockVertex));
    //vao->BindIndexBufferObject(ibo);
    glDrawElements(GL_TRIANGLES, frameChunkDrawCalls[i].indicesToDraw, GL_UNSIGNED_INT, 0);
#else // should draw from huge buffer
    VboMappedRangeRef<BlockQuad>* chunkVboRange = frameChunkDrawCalls[i].chunk->GetRenderComponent()->GetVboRange();
    //glDrawElementsBaseVertex(
    //  GL_TRIANGLES,
    //  frameChunkDrawCalls[i].indicesToDraw,
    //  GL_UNSIGNED_INT,
    //  nullptr,
    //  (GLint)chunkVboRange->GetOffset() * 4 // 4 vertices per quad (1 quad = 1 element)
    //);

    glDrawElementsInstancedBaseVertexBaseInstance(
      GL_TRIANGLES,
      frameChunkDrawCalls[i].indicesToDraw,
      GL_UNSIGNED_INT,
      nullptr,
      1,
      (GLint)chunkVboRange->GetOffset() * 4,
      0 // i
    );

#endif
  }

  engine->SwapGlfwBuffers();
}

void ChunkRenderer::MultidrawAllFrameChunks()
{
  const DrawCallData& boundDrawData = drawCalls[boundDrawCallId];
  blockMultidrawShader->Bind();
  blockMultidrawShader->SetUniformMat4f(cameraMvpUniform, boundDrawData.cameraMVP);

  const uint32 chunksToDraw = frameChunkDrawCalls.Size();

  darray<glm::vec3> chunkOffsets;
  darray<DrawElementsIndirectCommand> commands;
  chunkOffsets.Reserve(chunksToDraw);
  commands.Reserve(chunksToDraw);

  for (uint32 i = 0; i < chunksToDraw; i++) {
    const VboMappedRangeRef<BlockQuad>* chunkVboRange = frameChunkDrawCalls[i].chunk->GetRenderComponent()->GetVboRange();
    DrawElementsIndirectCommand command;
    command.count = frameChunkDrawCalls[i].indicesToDraw;
    command.instanceCount = 1;
    command.firstIndex = 0;
    command.baseVertex = chunkVboRange->GetOffset() * 4;
    command.baseInstance = i;
    chunkOffsets.Add(frameChunkDrawCalls[i].chunkPositionOffset);
    commands.Add(command);
  }

  multidrawChunkOffsets->BufferData(chunkOffsets.Data(), chunksToDraw);

  multidrawVao->Bind();

  multidrawVao->BindIndexBufferObject(blocksIbo);
  multidrawVao->BindVertexBufferObject(hugeVbo->GetVbo(), sizeof(BlockVertex));
  glVertexArrayVertexBuffer(multidrawVao->GetId(), 4, multidrawChunkOffsets->GetId(), 0, sizeof(glm::vec3));

  multidrawCommands->BufferData(commands.Data(), chunksToDraw);
  multidrawCommands->Bind();

  glMultiDrawElementsIndirect(
    GL_TRIANGLES,
    GL_UNSIGNED_INT,
    (const void*)0,
    chunksToDraw,
    0
  );

  engine->SwapGlfwBuffers();
}
/*
void ChunkRenderer::GeneratedMultidrawIndirectCommands()
{
  const uint32 chunkCount = frameChunkDrawCalls.Size();
  if (dibos->GetCapacity() < chunkCount) {
    dibos->Reserve(chunkCount);
  }
  if (multidrawChunkOffsets->GetCapacity() < chunkCount) {
    multidrawChunkOffsets->Reserve(chunkCount);
  }

  //DrawElementsIndirectCommand* commands = new DrawElementsIndirectCommand[chunkCount];
  //glm::vec3* chunkOffsets = new glm::vec3[chunkCount];

  //DrawElementsIndirectCommand* bufferCommands = dibos->GetModifyMapped().data;
  //glm::vec3* bufferChunkOffsets = multidrawChunkOffsets->GetModifyMapped().data;

  //DrawElementsIndirectCommand* bufferCommands = multidrawCommandsBuffer->GetMapBuffer(GLMappedBufferAccess::WriteOnly);
  //glm::vec3* bufferChunkOffsets = multidrawChunkOffsetsBuffer->GetMapBuffer<glm::vec3>(GLMappedBufferAccess::WriteOnly);

  frameMultidrawChunkOffsets.Empty();
  frameMultidrawCommands.Empty();

  frameMultidrawChunkOffsets.Reserve(chunkCount);
  frameMultidrawCommands.Reserve(chunkCount);

  for (uint32 i = 0; i < chunkCount; i++) {
    glm::vec3 chunkOffset = frameChunkDrawCalls[i].chunkPositionOffset;

    const VboMappedRangeRef<BlockQuad>* chunkVboRange = frameChunkDrawCalls[i].chunk->GetRenderComponent()->GetVboRange();

    DrawElementsIndirectCommand command;
    command.count = frameChunkDrawCalls[i].indicesToDraw;
    command.instanceCount = 1;
    command.firstIndex = 0;
    command.baseVertex = chunkVboRange->GetOffset() * 4;
    command.baseInstance = i;

    //bufferCommands[i] = command;
    //bufferChunkOffsets[i] = chunkOffset;

    frameMultidrawCommands.Add(command);
    frameMultidrawChunkOffsets.Add(chunkOffset);

  }

  //multidrawCommandsBuffer->UnmapBuffer();
  //multidrawChunkOffsetsBuffer->UnmapBuffer();

  //dibos->SwapNextBuffer();
  //multidrawChunkOffsets->SwapNextBuffer();
}*/

void ChunkRenderer::RemoveChunkFromFrameDraw(Chunk* chunk)
{
  gk_assertm(false, "not implemented");
  ///frameChunkDrawCalls.RemoveFirst(chunk);
}

void ChunkRenderer::CreateGLBuffersForChunks(const darray<Chunk*>& chunks)
{
  assertOnRenderThread();
  for (Chunk* chunk : chunks) {
    ChunkRenderComponent* renderComponent = chunk->GetRenderComponent();

    ChunkMesh* mesh = renderComponent->GetMesh();
    gk_assertm(mesh->GetQuadCount() > 0, "Cannot allocate GL buffers for chunk with a mesh with 0 quads");

    if (!renderComponent->AreGLBuffersInitialized()) {
      renderComponent->CreateGLBuffers();
    }

    auto cvbos = renderComponent->GetVbos();
    auto cibos = renderComponent->GetIbos();

    if (cvbos->GetCapacity() < mesh->GetQuadCount()) {
      cvbos->Reserve(static_cast<int>(mesh->GetQuadCount() * 1.5));
    }
    if (cibos->GetCapacity() < mesh->GetIndexCount()) {
      cibos->Reserve(static_cast<int>(mesh->GetIndexCount() * 1.5));
    }
  }
}

glm::vec3 ChunkRenderer::GetChunkShaderPositionOffset(const glm::dvec3 playerPos, const Chunk* chunk)
{
  const ChunkPosition _cPos = chunk->GetPosition();
  const glm::dvec3 chunkLocation = glm::dvec3(_cPos.x * CHUNK_LENGTH, _cPos.y * CHUNK_LENGTH, _cPos.z * CHUNK_LENGTH);
  return glm::vec3(double(chunkLocation.x - playerPos.x), double(chunkLocation.y - playerPos.y), double(chunkLocation.z - playerPos.z));
}

VboMappedRangeRef<BlockQuad>* ChunkRenderer::CreateMappedVboRange(const uint64 elementCapacity)
{
  assertOnRenderThread();
  VboMappedRangeRef<BlockQuad>* newRange = hugeVbo->NewMappedRange(elementCapacity);
  return newRange;
}
