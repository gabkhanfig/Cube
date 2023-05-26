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
#include "Block/BlockTypes/Air/AirBlock.h"
#include "Raycast.h"

World* GetWorld()
{
  checkm(GetGameInstance()->GetWorld() != nullptr, "The world cannot be nullptr");
  return GetGameInstance()->GetWorld();
}

World::World()
{
  player = new Player();
  //player->SetLocation({ 0, 33, 0 });
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

  darray<Chunk*> remeshedChunks;
  for (auto& chunkPair : chunks) {
    Chunk* chunk = chunkPair.second;
    remeshedChunks.Add(chunk);
    //chunk->GetRenderComponent()->RecreateMesh();
  }
  ChunkRenderComponent::MultithreadRecreateMeshes(chunkRenderer, remeshedChunks);
  chunkRenderer->SetRemeshedChunks(remeshedChunks);
}

void World::Tick(float deltaTime)
{
  // Tick world
  player->Tick(deltaTime);

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

bool World::SetBlockAt(WorldPosition position, Block* block)
{
  if (!DoesChunkExist(position.ToChunkPosition())) {
    return false;
  }

  Chunk* chunk = GetChunk(position.ToChunkPosition());
  chunk->SetBlockAt(position.ToBlockPosition(), block);
  return true;
}

void World::DestroyBlockAt(WorldPosition position)
{
  if (!DoesChunkExist(position.ToChunkPosition())) {
    return;
  }
  Chunk* chunk = GetChunk(position.ToChunkPosition());
  chunk->DestroyBlockAt(position.ToBlockPosition());
}

bool World::DoesChunkExist(ChunkPosition position) const
{
  return chunks.contains(position);
}

bool World::DoesBlockExist(WorldPosition position) const
{
  return chunks.contains(position.ToChunkPosition());
}


RaycastHitResult World::RaycastHit(const glm::dvec3 start, const glm::dvec3 end) const
{
  const GlobalString airName = AirBlock::GetStaticName();

  const glm::dvec3 rayDirection = glm::normalize(end - start);

  glm::dvec3 pos = glm::floor(start);
  const glm::dvec3 step = glm::sign(rayDirection);
  const glm::dvec3 tDelta = step / rayDirection;
  const glm::dvec3 fr = glm::fract(start);

  glm::dvec3 tMax;
  tMax.x = tDelta.x * ((start.x > 0.0) ? (1.0 - fr.x) : fr.x);
  tMax.y = tDelta.y * ((start.y > 0.0) ? (1.0 - fr.y) : fr.y);
  tMax.z = tDelta.z * ((start.z > 0.0) ? (1.0 - fr.z) : fr.z);

  glm::dvec3 norm = glm::vec3(0, -step.y, 0);

  // If the component start is greater than the component end, passing it would mean the current step position is less than the end, otherwise greater
#define COMPONENT_PASSED(n) (start.n > end.n ? (pos.n - 0.5) < end.n : (pos.n + 0.5) > end.n)

  RaycastHitResult result;

  while (!COMPONENT_PASSED(x) || !COMPONENT_PASSED(y) || !COMPONENT_PASSED(z)) {
    const WorldPosition wp{ pos };
    Block* block = GetBlock(wp);
    if (block != nullptr && block->GetName() != airName) {
      result.success = RaycastHitResult::HitSuccess::block;
      result.hitBlock = block;
      result.position = pos;
      result.normal = norm;
      return result;
    }

    if (tMax.x < tMax.y) {
      if (tMax.z < tMax.x) {
        tMax.z += tDelta.z;
        pos.z += step.z;
        norm = glm::vec3(0, 0, -step.z);
      }
      else {
        tMax.x += tDelta.x;
        pos.x += step.x;
        norm = glm::vec3(-step.x, 0, 0);
      }
    }
    else {
      if (tMax.z < tMax.y) {
        tMax.z += tDelta.z;
        pos.z += step.z;
        norm = glm::vec3(0, 0, -step.z);
      }
      else {
        tMax.y += tDelta.y;
        pos.y += step.y;
        norm = glm::vec3(0, -step.y, 0);
      }
    }

  }

  return result;
#undef COMPONENT_PASSED
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
  darray<Chunk*> chunksToAttemptDraw;
  for (auto& chunkPair : chunks) {
    // Check if chunk can be drawn
    chunksToAttemptDraw.Add(chunkPair.second);
  }
  // 2. Wait until previous render execution is finished.
  gk::Thread* renderThread = engine->GetRenderThread();
  if (usingRenderThread) {
    auto startWait = std::chrono::high_resolution_clock::now();
#ifdef DEVELOPMENT
    while (!renderThread->IsReady()) {
      std::chrono::steady_clock::time_point now = std::chrono::high_resolution_clock::now();
      auto duration = now - startWait; 
      if (std::chrono::duration_cast<std::chrono::seconds>(duration).count() > 5) {
        cubeLog("stuck waiting for render thread");
        DebugBreak();
      }
#else 
    while (!renderThread->IsReady());
#endif
    }
  }
  
  // 3. Remove any chunks that shouldn't be drawn from the ChunkRenderer's drawChunks map.

  // 4. From the chunks to be drawn, find which ones need to be remeshed.
  darray<Chunk*> remeshedChunks;
  for (Chunk* chunk : chunksToAttemptDraw) {
    if (!chunk->ShouldBeRemeshed()) continue;
    //cubeLog("chunk should be remeshed at: ");
    //std::cout << chunk->GetPosition().x << ", " << chunk->GetPosition().y << ", " << chunk->GetPosition().z << '\n';
    remeshedChunks.Add(chunk);
  }
  // 5. Remesh each chunk, passing in the mapped ChunkRenderMeshData structure to each chunk render component.
  if (remeshedChunks.Size() > 0) {
    ChunkRenderComponent::MultithreadRecreateMeshes(chunkRenderer, remeshedChunks);
    chunkRenderer->SetRemeshedChunks(remeshedChunks);
  }

  // 6. Filter out any empty chunks
  darray<Chunk*> chunksToDraw;
  for (Chunk* chunk : chunksToAttemptDraw) {
    if (!chunk->GetRenderComponent()->IsMeshEmpty()) chunksToDraw.Add(chunk);
  }
  
  // 7. Store player position data, camera mvp matrix, and any other data that may change while drawing is occurring within the ChunkRenderer.
  chunkRenderer->StoreModifyDrawCallData();
  // 8. Execute whole world draw task on the render thread.
  if (!usingRenderThread) {
    chunkRenderer->DrawAllChunksAndPrepareNext(chunksToDraw);
  }
  else {
    auto func = std::bind(&ChunkRenderer::DrawAllChunksAndPrepareNext, chunkRenderer, chunksToDraw);
    renderThread->BindFunction(func);
    renderThread->Execute();
    //while (!renderThread->IsReady());
  }
  
}
