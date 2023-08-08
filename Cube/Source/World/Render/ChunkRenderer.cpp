#include "ChunkRenderer.h"
#include "../../Graphics/OpenGL/Shader/RasterShader.h"
#include "../../Graphics/OpenGL/Shader/ComputeShader.h"
#include "../../Graphics/OpenGL/Buffers/VertexBufferObject.h"
#include "../../Graphics/OpenGL/Buffers/IndexBufferObject.h"
#include "../../Graphics/OpenGL/Buffers/VertexArrayObject.h"
#include "../../Graphics/OpenGL/Buffers/ShaderBufferObject.h"
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

ChunkRenderer::ChunkRenderer()
  : chunkOffsetUniform("u_chunkOffset"), cameraMvpUniform("u_cameraMVP"), boundDrawCallId(0), modifyDrawCallId(1)
{
  //shader = new Shader(CompileTimeFiles::GetTextFile("Chunk.vert")->contents, CompileTimeFiles::GetTextFile("Chunk.frag")->contents);
  blockShader = new RasterShader(CompileTimeFiles::GetTextFile("Block.vert")->contents, CompileTimeFiles::GetTextFile("Block.frag")->contents);
  vao = new VertexArrayObject();
  vao->SetFormatLayout(BlockQuad::GetQuadsVertexBufferLayout());
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

void ChunkRenderer::BindBlocksVertexBufferObject(VertexBufferObject* vbo)
{
  vao->BindVertexBufferObject(vbo, sizeof(BlockVertex));
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

void ChunkRenderer::DrawAllChunksAndPrepareNext(const darray<Chunk*>& chunksToDrawNextFrame)
{
  // 8. Draw the chunks
  const DrawCallData& boundDrawData = drawCalls[boundDrawCallId];
  PerformBoundDrawCalls();

  DrawCallData& modifyDrawData = drawCalls[modifyDrawCallId];

  darray<BlockQuad> quads;
  darray<uint32> indices;
  uint32 baseVertex = 0;

  // 9. Copy the remeshed chunks to their OpenGL buffers.
  for (Chunk* chunk : remeshedChunks) {
    ChunkRenderComponent* renderComponent = chunk->GetRenderComponent();

    ChunkMesh* mesh = renderComponent->GetMesh();
    if (mesh->GetQuadCount() == 0) continue;

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

    cvbos->SwapNextBuffer();
    cibos->SwapNextBuffer();
  }
  remeshedChunks.Empty();

  // 10. Prepare the draw calls for the next frame.
  frameChunkDrawCalls.Empty();
  for (Chunk* chunk : chunksToDrawNextFrame) {
    frameChunkDrawCalls.Add(chunk);
  }

  // 11. Swap buffers
  SwapNextBuffers();
}

void ChunkRenderer::SetRemeshedChunks(const darray<Chunk*> newRemeshedChunks)
{
  remeshedChunks = newRemeshedChunks;
}

void ChunkRenderer::PerformBoundDrawCalls()
{
  Renderer::Clear();

  if (frameChunkDrawCalls.Size() == 0) {
    engine->SwapGlfwBuffers();
    return;
  }

  const DrawCallData& boundDrawData = drawCalls[boundDrawCallId];
  blockShader->Bind();
  SetShaderCameraMVP(boundDrawData.cameraMVP);

  for (uint32 i = 0; i < frameChunkDrawCalls.Size(); i++) {
    DrawChunk(frameChunkDrawCalls[i]);
  }

  engine->SwapGlfwBuffers();
}

void ChunkRenderer::DrawChunk(const Chunk* drawChunk)
{
  VertexBufferObject* vbo = drawChunk->GetRenderComponent()->GetVbos()->GetBoundBuffer();
  IndexBufferObject* ibo = drawChunk->GetRenderComponent()->GetIbos()->GetBoundBuffer();
  if (vbo == nullptr || ibo == nullptr) return;
  if (ibo->GetIndexCount() == 0) return;

  BindBlocksVertexBufferObject(vbo);
  SetShaderChunkOffset(GetChunkShaderPositionOffset(drawCalls[boundDrawCallId].playerPos, drawChunk));
  Renderer::DrawVboTriangles(vbo, ibo);
}

void ChunkRenderer::RemoveChunkFromFrameDraw(Chunk* chunk)
{
  frameChunkDrawCalls.RemoveFirst(chunk);
}

glm::vec3 ChunkRenderer::GetChunkShaderPositionOffset(const glm::dvec3 playerPos, const Chunk* chunk)
{
  const ChunkPosition _cPos = chunk->GetPosition();
  const glm::dvec3 chunkLocation = glm::dvec3(_cPos.x * CHUNK_LENGTH, _cPos.y * CHUNK_LENGTH, _cPos.z * CHUNK_LENGTH);
  return glm::vec3(double(chunkLocation.x - playerPos.x), double(chunkLocation.y - playerPos.y), double(chunkLocation.z - playerPos.z));
}
