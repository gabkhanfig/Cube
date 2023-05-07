#include "World.h"
#include "../GameInstance.h"
#include "../Player/Player.h"
#include "../Graphics/OpenGL/Camera/Camera.h"
#include "../Graphics/OpenGL/Shader/Shader.h"
#include "../Graphics/OpenGL/Buffers/VertexBufferObject.h"
#include "../Graphics/OpenGL/Buffers/IndexBufferObject.h"
#include "../Graphics/OpenGL/Buffers/VertexArrayObject.h"
#include "../Graphics/OpenGL/Buffers/ShaderBufferObject.h"
#include "../Graphics/OpenGL/Buffers/DrawIndirectBufferObject.h"
#include "../../Resources/GeneratedFiles.h"
#include "../Graphics/Geometry/BlockGeometry.h"
#include <glad/glad.h>
#include "Block/BlockTextureAtlas.h"
#include "../Graphics/OpenGL/Render/Renderer.h"
#include "../Graphics/Geometry/ChunkMesh.h"
#include "Block/BlockFactory.h"
#include "Block/Block.h"
#include "Chunk/Chunk.h"
#include "Chunk/ChunkRenderComponent.h"
#include "Render/ChunkRenderer.h"
#include <chrono>
#include "../Engine/Engine.h"
#include "Terrain/TerrainGenerator.h"

World* GetWorld()
{
  checkm(GetGameInstance()->GetWorld() != nullptr, "The world cannot be nullptr");
  return GetGameInstance()->GetWorld();
}

World::World()
{
  player = new Player();
  player->SetLocation({ 0, 33, 0 });
  if (!engine->IsUsingRenderThread()) {
    CreateChunkRenderer();
  }
  else {
    gk::Thread* renderThread = engine->GetRenderThread();
    auto func = std::bind(&World::CreateChunkRenderer, this);
    renderThread->BindFunction(func);
    renderThread->Execute();
    while (!renderThread->IsReady());
  }

  terrainGenerator = new TerrainGenerator();
}

void World::BeginWorld()
{
  const int renderDistance = 2;
  const int lower = -2;

  for (int x = lower; x < renderDistance; x++) {
    for (int y = lower; y < renderDistance; y++) {
      for (int z = lower; z < renderDistance; z++) {
        Chunk* chunk = new Chunk({ x, y, z });
        chunk->GenerateTerrain(terrainGenerator);
        //chunk->FillChunkWithBlock("stoneBlock");
        chunks.insert({ chunk->GetPosition(), chunk });
      }
    }
  }

  darray<ChunkRenderComponent*> chunkRenderComponents;
  darray<Chunk*> remeshedChunks;
  for (auto& chunkPair : chunks) {
    Chunk* chunk = chunkPair.second;
    chunkRenderComponents.Add(chunk->GetRenderComponent());
    remeshedChunks.Add(chunk);
    //chunk->GetRenderComponent()->RecreateMesh();
  }
  ChunkRenderComponent::MultithreadRecreateMeshes(chunkRenderer, chunkRenderComponents);
  chunkRenderer->SetRemeshedChunks(remeshedChunks);
}

void World::Tick(float deltaTime)
{
  // Tick world

  RenderLoop();
}

Chunk* World::GetChunk(ChunkPosition position) const
{
  auto found = chunks.find(position);
  if (found == chunks.end()) {
    return nullptr;
  }
  return found->second;
}

Block* World::GetBlock(WorldPosition position) const
{
  ChunkPosition cpos;
  BlockPosition bpos;
  position.ToChunkAndBlock(&cpos, &bpos);
  const Chunk* chunk = GetChunk(cpos);
  if (chunk == nullptr) return nullptr;
  return chunk->GetBlock(bpos);
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
  //Renderer::Clear();

  //chunkRenderer->DrawAllChunks(chunks);
  //chunkRenderer->MultidrawIndirectAllChunks(chunks);

  engine->SwapGlfwBuffers();
}

void World::CreateChunkRenderer()
{
  chunkRenderer = new ChunkRenderer();
}

void World::RenderLoop()
{
  const bool usingRenderThread = engine->IsUsingRenderThread();
  // Render Loop
  //if (!engine->IsUsingRenderThread()) {
  //  Renderer::Clear();
  //  //DrawWorld();
  //  return;
  //}

  // 1. Get all of the chunks, including LOD chunks that should be drawn.
  darray<Chunk*> chunksToDraw;
  for (auto& chunkPair : chunks) {
    // Check if chunk can be drawn
    chunksToDraw.Add(chunkPair.second);
  }
  // 2. Wait until previous render execution is finished.
  gk::Thread* renderThread = engine->GetRenderThread();
  if (usingRenderThread) {
    while (!renderThread->IsReady());
  }
  
  // 3. Remove any chunks that shouldn't be drawn from the ChunkRenderer's drawChunks map.

  // 4. From the chunks to be drawn, find which ones need to be remeshed.

  // 5. Remesh each chunk, passing in the mapped ChunkRenderMeshData structure to each chunk render component.

  // 6. Store player position data, camera mvp matrix, and any other data that may change while drawing is occurring within the ChunkRenderer.
  chunkRenderer->StoreModifyDrawCallData();
  // 7. Execute whole world draw task on the render thread.
  if (!usingRenderThread) {
    chunkRenderer->DrawAllChunksAndPrepareNext(chunksToDraw);
  }
  else {
    auto func = std::bind(&ChunkRenderer::DrawAllChunksAndPrepareNext, chunkRenderer, chunksToDraw);
    renderThread->BindFunction(func);
    renderThread->Execute();
    while (!renderThread->IsReady());
  }
  // 8. Copy all of the chunks mesh data within a single VBO and IBO using offsets within them. Will potentially need to reallocate the buffers.

  // 9. Iterate through each chunk, drawing it's data using the ChunkRenderMeshData.

  // 10. Swap buffers.
}
