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
#include "../../Engine/Engine.h"

ChunkRenderer::ChunkRenderer()
  : chunkOffsetUniform("u_chunkOffset"), cameraMvpUniform("u_cameraMVP"), boundDrawCallId(0), modifyDrawCallId(1)
{
  shader = new Shader(generated_Chunk_vert, generated_Chunk_frag);
  vao = new VertexArrayObject();
  vao->SetFormatLayout(BlockQuad::GetQuadsVertexBufferLayout());
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

ChunkMesh* ChunkRenderer::GetChunkMesh(Chunk* chunk) const
{
  auto found = buffers.find(chunk);
  check(found != buffers.end());
  return found->second.mesh;
}

void ChunkRenderer::AllocateMeshesForChunks(const HashMap<ChunkPosition, Chunk*>& chunks)
{
  for (const auto& chunkPair : chunks) {
    AllocateMeshForChunk(chunkPair.second);
  }
}

void ChunkRenderer::AllocateMeshForChunk(Chunk* chunk)
{
  if (buffers.contains(chunk)) return;
  ChunkBuffers cbuffers;
  cbuffers.vbos = new PersistentMappedTripleBuffer<VertexBufferObject, BlockQuad>();
  cbuffers.ibos = new PersistentMappedTripleBuffer<IndexBufferObject, uint32>();
  cbuffers.mesh = new ChunkMesh();
  buffers.insert({ chunk, cbuffers });
}

void ChunkRenderer::DrawAllChunksAndPrepareNext()
{
  const DrawCallData& boundDrawData = drawCalls[boundDrawCallId]; 
  PerformBoundDrawCalls();

  DrawCallData& modifyDrawData = drawCalls[modifyDrawCallId];

  darray<BlockQuad> quads;
  darray<uint32> indices;
  uint32 baseVertex = 0;

  // Copy the remeshed chunks to their OpenGL buffers.
  for (Chunk* chunk : remeshedChunks) {
    auto found = buffers.find(chunk);
    check(found != buffers.end());

    ChunkBuffers& cbuffers = found->second; 
    ChunkMesh* mesh = cbuffers.mesh;
    if (mesh->GetQuadCount() == 0) continue;

    auto cvbos = cbuffers.vbos;
    auto cibos = cbuffers.ibos;

    if (cvbos->GetCapacity() < mesh->GetQuadCount()) {
      cvbos->Reserve(mesh->GetQuadCount() * 1.5);
    }
    if (cibos->GetCapacity() < mesh->GetIndexCount()) {
      cibos->Reserve(mesh->GetIndexCount() * 1.5);
    }

    auto& mappedVbo = cvbos->GetModifyMapped();
    memcpy(mappedVbo.data, mesh->GetQuads().Data(), mesh->GetQuadCount() * sizeof(BlockQuad));
    auto& mappedIbo = cibos->GetModifyMapped();
    memcpy(mappedIbo.data, mesh->GetIndices().Data(), mesh->GetIndexCount() * sizeof(uint32));
    mappedIbo.buffer->SetIndexCount(mesh->GetIndexCount());

    cvbos->SwapNextBuffer();
    cibos->SwapNextBuffer();

    cbuffers.positionOffset = GetChunkShaderPositionOffset(modifyDrawData.playerPos, chunk);
  }
  remeshedChunks.Empty();

  // Prepare the draw calls for the next frame.
  frameChunkDrawCalls.Empty();
  for (const auto& bufferPair : buffers) {
    const ChunkBuffers cbuffers = bufferPair.second;

    ChunkDrawCall drawCall;
    drawCall.chunk = bufferPair.first;
    drawCall.vbos = cbuffers.vbos;
    drawCall.ibos = cbuffers.ibos;
    drawCall.positionOffset = cbuffers.positionOffset;
    frameChunkDrawCalls.Add(drawCall);
  }

  SwapNextBuffers();
}

void ChunkRenderer::SetRemeshedChunks(const darray<Chunk*> newRemeshedChunks)
{
  remeshedChunks = newRemeshedChunks;
}

void ChunkRenderer::PerformBoundDrawCalls()
{
  Renderer::Clear();

  if (frameChunkDrawCalls.Size() == 0) return;

  const DrawCallData& boundDrawData = drawCalls[boundDrawCallId];
  shader->Bind();
  SetShaderCameraMVP(boundDrawData.cameraMVP);

  for (int i = 0; i < frameChunkDrawCalls.Size(); i++) {
    const ChunkDrawCall& drawCall = frameChunkDrawCalls[i];
    DrawChunk(drawCall);
  }

  engine->SwapGlfwBuffers();
}

void ChunkRenderer::DrawChunk(const ChunkDrawCall& drawCall)
{
  VertexBufferObject* vbo = drawCall.vbos->GetBoundBuffer();
  IndexBufferObject* ibo = drawCall.ibos->GetBoundBuffer();
  if (vbo == nullptr || ibo == nullptr) return;

  BindBlocksVertexBufferObject(vbo);
  SetShaderChunkOffset(drawCall.positionOffset);
  Renderer::DrawVboTriangles(vbo, ibo);
}

glm::vec3 ChunkRenderer::GetChunkShaderPositionOffset(const glm::dvec3 playerPos, const Chunk* chunk)
{
  const ChunkPosition _cPos = chunk->GetPosition();
  const glm::dvec3 chunkLocation = glm::dvec3(_cPos.x * CHUNK_LENGTH, _cPos.y * CHUNK_LENGTH, _cPos.z * CHUNK_LENGTH);
  return glm::vec3(double(chunkLocation.x - playerPos.x), double(chunkLocation.y - playerPos.y), double(chunkLocation.z - playerPos.z));
}
