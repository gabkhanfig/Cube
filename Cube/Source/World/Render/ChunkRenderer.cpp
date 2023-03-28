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

ChunkRenderer::ChunkRenderer()
  : chunkOffsetUniform("u_chunkOffset"), cameraMvpUniform("u_cameraMVP")
{
  shader = new Shader(generated_Chunk_vert, generated_Chunk_frag);
  vao = new VertexArrayObject();
  vao->SetFormatLayout(BlockQuad::GetQuadsVertexBufferLayout());
  multidrawVbos = new PersistentMappedTripleVbo<BlockQuad>();
  multidrawIbos = new PersistentMappedTripleIbo();
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

  multidrawVbos->Reserve(vbosCapacity);
  multidrawIbos->Reserve(ibosCapacity);
}

void ChunkRenderer::SwapNextBuffer()
{
  multidrawVbos->SwapNextBuffer();
  multidrawIbos->SwapNextBuffer();
}
