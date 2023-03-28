#include "World.h"
#include "../GameInstance.h"
#include "../Player/Player.h"
#include "../Graphics/OpenGL/Camera/Camera.h"
#include "../Graphics/OpenGL/Shader/Shader.h"
#include "../Graphics/OpenGL/Buffers/VertexBufferObject.h"
#include "../Graphics/OpenGL/Buffers/IndexBufferObject.h"
#include "../Graphics/OpenGL/Buffers/VertexArrayObject.h"
#include "../Graphics/OpenGL/Buffers/ShaderBufferObject.h"
#include "../../Resources/GeneratedFiles.h"
#include "../Graphics/Geometry/BlockGeometry.h"
#include <glad/glad.h>
#include "Block/BlockTextureAtlas.h"
#include "../Graphics/OpenGL/Render/Renderer.h"
#include "../Graphics/Geometry/ChunkMesh.h"
#include "Block/BlockFactory.h"
#include "Block/IBlock.h"
#include "Chunk/Chunk.h"
#include "Chunk/ChunkRenderComponent.h"
#include "Render/ChunkRenderer.h"
#include <chrono>

World* GetWorld()
{
  checkm(GetGameInstance()->GetWorld() != nullptr, "The world cannot be nullptr");
  return GetGameInstance()->GetWorld();
}

World::World()
{
  player = new Player();
  chunkRenderer = new ChunkRenderer();
}

void World::BeginWorld()
{
  const int c = 1;
  for (int x = 0; x < 1; x++) {
    for (int y = 0; y < 1; y++) {
      for (int z = 0; z < 1; z++) {
        Chunk* chunk = new Chunk({ x, y, z });
        chunk->FillChunkWithBlock("stoneBlock");
        chunks.insert({ chunk->GetPosition(), chunk });
      }
    }
  }

  chunkRenderer->ReserveVbosAndIbosForChunkQuantity(chunks.size());

  darray<ChunkRenderComponent*> chunkRenderComponents;
  for (auto& chunkPair : chunks) {
    Chunk* chunk = chunkPair.second;
    chunkRenderComponents.Add(chunk->GetRenderComponent());
  }
  ChunkRenderComponent::MultithreadRecreateMeshes(chunkRenderComponents);
}

void World::Tick(float deltaTime)
{
  DrawWorld();
}

Chunk* World::GetChunk(ChunkPosition position) const
{
  auto found = chunks.find(position);
  if (found == chunks.end()) {
    return nullptr;
  }
  return found->second;
}

IBlock* World::GetBlock(WorldPosition position) const
{
  ChunkPosition cpos;
  BlockPosition bpos;
  position.ToChunkAndBlock(&cpos, &bpos);
  const Chunk* chunk = GetChunk(cpos);
  if (chunk == nullptr) return nullptr;
  return chunk->ChunkBlockAt(bpos)->GetBlock();
}

ChunkBlock* World::ChunkBlockAt(WorldPosition position) const
{
  ChunkPosition cpos;
  BlockPosition bpos;
  position.ToChunkAndBlock(&cpos, &bpos);
  const Chunk* chunk = GetChunk(cpos);
  if (chunk == nullptr) return nullptr; 
  return chunk->ChunkBlockAt(bpos);
}

bool World::DoesChunkExist(ChunkPosition position) const
{
  return chunks.contains(position);
}

bool World::DoesBlockExist(WorldPosition position) const
{
  return chunks.contains(position.ToChunkPosition());
}

void World::DrawWorld()
{
  /* Render here */
  Renderer::Clear();

  //chunkShader->Bind();
  Camera* cam = Camera::GetActiveCamera();
  chunkRenderer->SetShaderCameraMVP(cam->GetMvpMatrix());

  //for (auto& chunkPair : chunks) {
  //  Chunk* chunk = chunkPair.second;
  //  chunk->Draw(chunkRenderer);
  //}
  
  Chunk* chunk = chunks.find(ChunkPosition(0, 0, 0))->second;
  ChunkRenderComponent* renderComponent = chunk->GetRenderComponent();

  chunkRenderer->SetShaderChunkOffset(chunkRenderer->GetOffsetForChunkDraw(chunk));

  uint32 gIndirectBuffer;
  glGenBuffers(1, &gIndirectBuffer);
  glBindBuffer(GL_DRAW_INDIRECT_BUFFER, gIndirectBuffer);
  DrawElementsIndirectCommand cmd = renderComponent->GenerateDrawElementsIndirectCommand();
  glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(DrawElementsIndirectCommand), &cmd, GL_DYNAMIC_DRAW);

  PersistentMappedTripleVbo<BlockQuad>::MappedVbo& mappedVbo = chunkRenderer->GetMultidrawVbos()->GetModifyMappedVbo();
  const ChunkMesh& mesh = renderComponent->GetMesh();
  mesh.CopyQuadsToBuffer(mappedVbo.data);

  PersistentMappedTripleIbo::MappedIbo& mappedIbo = chunkRenderer->GetMultidrawIbos()->GetModifyMappedIbo();
  mesh.CopyIndicesToBuffer(mappedIbo.data, 0);
  mappedIbo.ibo->SetIndexCount(mesh.GetIndexCount());

  //chunkRenderer->GetMultidrawVbos()->GetBoundVbo()->Bind();
  //chunkRenderer->GetMultidrawIbos()->GetBoundIbo()->Bind();

  VertexBufferObject* boundVbo = chunkRenderer->GetMultidrawVbos()->GetBoundVbo();
  IndexBufferObject* boundIbo = chunkRenderer->GetMultidrawIbos()->GetBoundIbo();
  chunkRenderer->BindBlocksVertexBufferObject(boundVbo);
  boundVbo->Bind();
  boundIbo->Bind();

  glMultiDrawElementsIndirect(GL_TRIANGLES, //type
    GL_UNSIGNED_INT, //indices represented as unsigned ints
    (void*)0, //start with the first draw command
    1, //draw n chunks
    0); //no stride, the draw commands are tightly packed
  //glDrawElements(GL_TRIANGLES, boundIbo->GetIndexCount(), GL_UNSIGNED_INT, 0);

  chunkRenderer->SwapNextBuffer();
}
