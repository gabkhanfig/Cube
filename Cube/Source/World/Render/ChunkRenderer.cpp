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
#include <glad/glad.h>

ChunkRenderer::ChunkRenderer()
  : chunkOffsetUniform("u_chunkOffset"), cameraMvpUniform("u_cameraMVP")
{
  shader = new Shader(generated_Chunk_vert, generated_Chunk_frag);
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

void ChunkRenderer::SwapNextBuffer()
{
  multidrawVbos->SwapNextBuffer();
  multidrawIbos->SwapNextBuffer();
  multidrawIndirectBuffers->SwapNextBuffer();
}

void ChunkRenderer::DrawAllChunks(const HashMap<ChunkPosition, Chunk*>& chunks)
{
  for (auto& chunkPair : chunks) {
    Chunk* chunk = chunkPair.second;
    chunk->Draw(this);
  }
}

void ChunkRenderer::MultidrawIndirectAllChunks(const HashMap<ChunkPosition, Chunk*>& chunks)
{
  Chunk* chunk = chunks.find(ChunkPosition(0, 0, 0))->second;
  ChunkRenderComponent* renderComponent = chunk->GetRenderComponent();

  SetShaderChunkOffset(GetOffsetForChunkDraw(chunk));

  //DrawElementsIndirectCommand cmd = renderComponent->GenerateDrawElementsIndirectCommand();

  //DrawIndirectBufferObject* dbo = DrawIndirectBufferObject::Create(&cmd, 1);

  PersistentMappedTripleVbo<BlockQuad>::MappedVbo& mappedVbo = multidrawVbos->GetModifyMappedVbo();
  renderComponent->CopyMeshQuadsToVboOffset(mappedVbo, 0);

  PersistentMappedTripleIbo::MappedIbo& mappedIbo = multidrawIbos->GetModifyMappedIbo();
  renderComponent->CopyMeshIndicesToIboOffset(mappedIbo, 0, 0);

  PersistentMappedTripleIndirect::MappedIndirect& mappedDibo = multidrawIndirectBuffers->GetModifyMappedDibo();
  renderComponent->CopyDrawCommandToIndirectOffset(mappedDibo, 0);

  VertexBufferObject* boundVbo = multidrawVbos->GetBoundVbo();
  IndexBufferObject* boundIbo = multidrawIbos->GetBoundIbo();
  DrawIndirectBufferObject* boundIndirect = multidrawIndirectBuffers->GetBoundBufferObject();
  BindBlocksVertexBufferObject(boundVbo);
  boundVbo->Bind();
  boundIbo->Bind();
  boundIndirect->Bind();

  glMultiDrawElementsIndirect(GL_TRIANGLES, //type
    GL_UNSIGNED_INT, //indices represented as unsigned ints
    (void*)0, //start with the first draw command
    1, //draw n chunks
    0); //no stride, the draw commands are tightly packed
  //glDrawElements(GL_TRIANGLES, boundIbo->GetIndexCount(), GL_UNSIGNED_INT, 0);

  SwapNextBuffer();
}
