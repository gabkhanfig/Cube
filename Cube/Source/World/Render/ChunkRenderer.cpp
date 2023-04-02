#include "ChunkRenderer.h"
#include "../../Graphics/OpenGL/Shader/Shader.h"
#include "../../Graphics/OpenGL/Buffers/VertexBufferObject.h"
#include "../../Graphics/OpenGL/Buffers/IndexBufferObject.h"
#include "../../Graphics/OpenGL/Buffers/VertexArrayObject.h"
#include "../../Graphics/OpenGL/Buffers/ShaderBufferObject.h"
#include "../../../Resources/GeneratedFiles.h"
#include "../../Graphics/Geometry/BlockGeometry.h"
#include "../WorldTransform.h"
#include "../World.h"
#include "../../Player/Player.h"
#include "../Chunk/Chunk.h"
#include "../../Graphics/OpenGL/Render/Renderer.h"
#include "../Chunk/ChunkRenderComponent.h"
#include "../../Graphics/OpenGL/Camera/Camera.h"
#include <glad/glad.h>

ChunkRenderer::ChunkRenderer()
  : chunkOffsetUniform("u_chunkOffset"), cameraMvpUniform("u_cameraMVP")
{
  shader = new Shader(generated_Chunk_vert, generated_Chunk_frag);
  multidrawShader = new Shader(generated_ChunkMultidraw_vert, generated_ChunkMultidraw_frag);
  vao = new VertexArrayObject();
  vao->SetFormatLayout(BlockQuad::GetQuadsVertexBufferLayout());
  multidrawVbos = new PersistentMappedTripleVbo<BlockQuad>();
  multidrawIbos = new PersistentMappedTripleIbo();
  multidrawIndirectBuffers = new PersistentMappedTripleIndirect();
}

glm::vec3 ChunkRenderer::GetOffsetForChunkDraw(const Chunk* chunk) const
{
  const glm::dvec3 playerLocation = GetWorld()->GetPlayer()->GetLocation();
  const ChunkPosition _cPos = chunk->GetPosition();
  const glm::dvec3 chunkLocation = glm::dvec3(_cPos.x * CHUNK_LENGTH, _cPos.y * CHUNK_LENGTH, _cPos.z * CHUNK_LENGTH);
  return glm::vec3(double(chunkLocation.x - playerLocation.x), double(chunkLocation.y - playerLocation.y), double(chunkLocation.z - playerLocation.z));
}

void ChunkRenderer::SetShaderChunkOffset(glm::vec3 chunkOffset)
{
  shader->SetUniform3f(chunkOffsetUniform, chunkOffset);
}

void ChunkRenderer::SetShaderCameraMVP(const glm::mat4& cameraMVP)
{
  shader->SetUniformMat4f(cameraMvpUniform, cameraMVP);
}

void ChunkRenderer::Bind()
{
  shader->Bind();
  vao->Bind();
}

void ChunkRenderer::BindBlocksVertexBufferObject(VertexBufferObject* vbo)
{
  vao->BindVertexBufferObject(vbo, sizeof(BlockVertex));
}

void ChunkRenderer::Draw(VertexBufferObject* vbo, IndexBufferObject* ibo)
{
  Renderer::DrawVboTriangles(vbo, ibo);
}

void ChunkRenderer::ReserveVbosAndIbosForChunkQuantity(uint32 chunksNum)
{
  check(chunksNum > 0);
  const uint32 vbosCapacity = chunksNum * ChunkRenderComponent::GetMaximumQuadsPerChunkMesh();
  const uint32 ibosCapacity = chunksNum * ChunkRenderComponent::GetMaximumIndicesPerChunkMesh();
  const uint32 indirectCapacity = chunksNum;

  multidrawVbos->Reserve(vbosCapacity);
  multidrawIbos->Reserve(ibosCapacity);
  multidrawIndirectBuffers->Reserve(indirectCapacity);
}

void ChunkRenderer::SwapNextBuffers()
{
  multidrawVbos->SwapNextBuffer();
  multidrawIbos->SwapNextBuffer();
  multidrawIndirectBuffers->SwapNextBuffer();
}

void ChunkRenderer::DrawAllChunks(const HashMap<ChunkPosition, Chunk*>& chunks)
{
  shader->Bind();
  Camera* cam = Camera::GetActiveCamera();
  SetShaderCameraMVP(cam->GetMvpMatrix());
  for (auto& chunkPair : chunks) {
    Chunk* chunk = chunkPair.second;
    ChunkRenderComponent* renderComponent = chunk->GetRenderComponent();
    renderComponent->Draw(this);
  }
}

void ChunkRenderer::MultidrawIndirectAllChunks(const HashMap<ChunkPosition, Chunk*>& chunks)
{
  //https://github.com/litasa/Advanced-OpenGL-Examples/blob/master/src/MultidrawIndirect/MultidrawIndirect.cpp

  multidrawShader->Bind(); // Use the multidraw shader
  Camera* cam = Camera::GetActiveCamera(); 
  multidrawShader->SetUniformMat4f(cameraMvpUniform, cam->GetMvpMatrix()); // Set the u_cameraMVP matrix in ChunkMultidraw.vert

  uint32 chunkIndex = 0;
  uint32 baseQuad = 0;
  uint32 indexOffset = 0;

  darray<DrawElementsIndirectCommand> indirectCommands;
  //darray<glm::vec3> chunkOffsets;

  for (const auto& chunkPair : chunks) {

    // TODO add chunk offsets as a mat4 for vertex shader

    const Chunk* chunk = chunkPair.second;
    ChunkRenderComponent* renderComponent = chunk->GetRenderComponent();

    indirectCommands.Add(renderComponent->GenerateDrawElementsIndirectCommand(
      baseQuad * 4, // Starting from this vertex as an offset into the vbo (4 vertices per quad)
      chunkIndex)); // gl_InstanceID
    //chunkOffsets.Add(GetOffsetForChunkDraw(chunk));

    PersistentMappedTripleVbo<BlockQuad>::MappedVbo& mappedVbo = multidrawVbos->GetModifyMappedVbo();
    renderComponent->CopyMeshQuadsToVboOffset(mappedVbo, baseQuad); // Copy the mesh data into the modifiable vbo starting at baseQuad offset into the memory

    PersistentMappedTripleIbo::MappedIbo& mappedIbo = multidrawIbos->GetModifyMappedIbo();
    renderComponent->CopyMeshIndicesToIboOffset(mappedIbo, indexOffset); // Copy the indices into the modifiable ibo starting at indexOffset into the memory

    chunkIndex++; // Increment for the next chunk
    baseQuad += renderComponent->GetMesh().GetQuadCount(); // Increment the quad memory offset for the next chunk
    indexOffset += renderComponent->GetMesh().GetIndexCount(); // Increment the index memory offset for the next chunk
  }

  PersistentMappedTripleIndirect::MappedIndirect& mappedIndirect = multidrawIndirectBuffers->GetModifyMappedDibo();
  memcpy(mappedIndirect.data, indirectCommands.Data(), sizeof(DrawElementsIndirectCommand) * indirectCommands.Size()); // Copy the draw indirect commands into the modifiable buffer object

  VertexBufferObject* boundVbo = multidrawVbos->GetBoundVbo();
  IndexBufferObject* boundIbo = multidrawIbos->GetBoundIbo();
  DrawIndirectBufferObject* boundIndirect = multidrawIndirectBuffers->GetBoundBufferObject();
  BindBlocksVertexBufferObject(boundVbo); // Bind the vbo to the vao's format
  boundVbo->Bind();
  boundIbo->Bind(); // Bind the ibo
  boundIndirect->Bind(); // Bind the draw elements indirect commands

  //ShaderBufferObject* ssbo = ShaderBufferObject::Create<glm::vec3>(chunkOffsets.Data(), 2, 3);

  glMultiDrawElementsIndirect(
    GL_TRIANGLES,     //type
    GL_UNSIGNED_INT,  //indices represented as unsigned ints
    (void*)0,         //start with the first draw command
    chunkIndex,       //draw n chunks
    0);               //no stride, the draw commands are tightly packed

  SwapNextBuffers();
}
