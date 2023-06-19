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
#include "../Settings/Settings.h"

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
  const darray<ChunkPosition> positionsInRenderDistance = ChunkPositionsWithinRenderDistance(ChunkPosition(0, 0, 0), GetSettings()->GetRenderDistance());

  darray<Chunk*> remeshedChunks;
  for (ChunkPosition position : positionsInRenderDistance) {
    Chunk* chunk = new Chunk(position);
    chunk->GenerateTerrain(terrainGenerator);
    chunks.insert({ position, chunk });
    remeshedChunks.Add(chunk);
  }

  ChunkRenderComponent::MultithreadRecreateMeshes(chunkRenderer, remeshedChunks);
  chunkRenderer->SetRemeshedChunks(remeshedChunks);
}

void World::Tick(float deltaTime)
{
  ChunkPosition oldPlayerChunkPos = player->GetChunkPosition();

  // Tick world
  player->Tick(deltaTime);


  if (engine->IsUsingRenderThread()) {
    const int timeoutMS = 5000;
    engine->WaitForRenderThread(timeoutMS);
  }

  if (player->GetChunkPosition() != oldPlayerChunkPos) {
    DeleteDistantChunksAndLoadNearby(GetSettings()->GetRenderDistance());
    cubeLog("player moved across chunk border");
  }

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

static glm::dvec3 RaycastTDelta(glm::dvec3 start, glm::dvec3 end, glm::dvec3 step) {
  return  glm::dvec3(
    step.x != 0 ? std::min(step.x / (end.x - start.x), 1000000.0) : 1000000.0,
    step.y != 0 ? std::min(step.y / (end.y - start.y), 1000000.0) : 1000000.0,
    step.z != 0 ? std::min(step.z / (end.z - start.z), 1000000.0) : 1000000.0
  );
}

static glm::dvec3 RaycastTMax(glm::dvec3 start, glm::dvec3 step, glm::dvec3 tDelta) {
#define FRAC_NEGATIVE(n) (n - floor(n))
#define FRAC_POSITIVE(n) (1.0 - n + floor(n))
  glm::dvec3 tMax;
  tMax.x = ((step.x > 0.0) ? tDelta.x * FRAC_POSITIVE(start.x) : tDelta.x * FRAC_NEGATIVE(start.x));
  tMax.y = ((step.y > 0.0) ? tDelta.y * FRAC_POSITIVE(start.y) : tDelta.y * FRAC_NEGATIVE(start.y));
  tMax.z = ((step.z > 0.0) ? tDelta.z * FRAC_POSITIVE(start.z) : tDelta.z * FRAC_NEGATIVE(start.z));
  return tMax;
}

RaycastHitResult World::RaycastHit(glm::dvec3 start, glm::dvec3 end) const
{
  const GlobalString airName = AirBlock::GetStaticName();

  const glm::dvec3 rayDirection = end - start;
  const glm::dvec3 step = glm::sign(rayDirection);

  const glm::dvec3 tDelta = RaycastTDelta(start, end, step);
  glm::dvec3 tMax = RaycastTMax(start, step, tDelta);

  glm::dvec3 pos = glm::floor(start);
  glm::dvec3 norm = glm::vec3(0, 1, 0);

  // If the component start is greater than the component end, passing it would mean the current step position is less than the end, otherwise greater
#define COMPONENT_PASSED(n) (start.n > end.n ? (pos.n - 0.1) < end.n : (pos.n + 0.1) > end.n)

  RaycastHitResult result;

  while (!COMPONENT_PASSED(x) && !COMPONENT_PASSED(y) && !COMPONENT_PASSED(z)) {
    const WorldPosition wp{ pos };
    Block* block = GetBlock(wp);
    if (block != nullptr && block->GetName() != airName) {
      result.success = RaycastHitResult::HitSuccess::Block;
      result.hitObject = block;
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

void World::CreateChunkRenderer()
{
  chunkRenderer = new ChunkRenderer();
}

void World::RenderLoop()
{
  const bool usingRenderThread = engine->IsUsingRenderThread();
  // Render Loop

  // 1. Get all of the chunks, including LOD chunks that should be drawn.
  darray<Chunk*> chunksToAttemptDraw;
  for (auto& chunkPair : chunks) {
    // Check if chunk can be drawn
    chunksToAttemptDraw.Add(chunkPair.second);
  }
  // 2. Wait until previous render execution is finished.
  gk::Thread* renderThread = engine->GetRenderThread();
  
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

void World::DeleteDistantChunksAndLoadNearby(int renderDistance)
{
  const darray<ChunkPosition> nearbyPositions = World::ChunkPositionsWithinRenderDistance(player->GetChunkPosition(), renderDistance);

  HashSet<ChunkPosition> _nearbyPositionsSet;
  HashSet<ChunkPosition> _chunksAlreadyExist;

  for (const ChunkPosition& pos : nearbyPositions) { // Filter out all of the chunks that already exist, and put all of the nearby chunks into a set for fast access.
    _nearbyPositionsSet.insert(pos);
    if (DoesChunkExist(pos)) {
      _chunksAlreadyExist.insert(pos);
    }
  }

  darray<ChunkPosition> chunksToDelete;
  for (auto& chunkPair : chunks) { // Find what chunks are not nearby
    if (!_nearbyPositionsSet.contains(chunkPair.first)) {
      delete chunkPair.second;
      chunkRenderer->RemoveChunkFromFrameDraw(chunkPair.second);
      chunksToDelete.Add(chunkPair.first);
    }
  }
  for (const ChunkPosition& pos : chunksToDelete) { // Remove deleted chunks from hashmap
    chunks.erase(pos);
  }

  darray<Chunk*> newChunks; // Create the new chunk objects
  for (const ChunkPosition& pos : nearbyPositions) {
    if (!DoesChunkExist(pos)) {
      Chunk* chunk = new Chunk(pos);
      newChunks.Add(chunk);
      chunks.insert({ pos, chunk });
    }
  }

  Benchmark b = Benchmark("multithread generate terrain");
  Chunk::MultithreadGenerateTerrain(newChunks, GetGameInstance()->GetThreadPool(), terrainGenerator);
  b.End(Benchmark::TimeUnit::ms);
}

darray<ChunkPosition> World::ChunkPositionsWithinRenderDistance(ChunkPosition center, int renderDistance)
{
  // TODO this function is a little lopsided. Figure out why and fix later. This is not critical
  const ChunkPosition lower = ChunkPosition(center.x - renderDistance, center.y - renderDistance, center.z - renderDistance);
  const ChunkPosition upper = ChunkPosition(center.x + renderDistance, center.y + renderDistance, center.z + renderDistance);

  darray<ChunkPosition> positions;
  for (int x = lower.x; x < upper.x; x++) {
    for (int y = lower.y; y < upper.y; y++) {
      for (int z = lower.z; z < upper.z; z++) {
        int distanceBetween = static_cast<int>(sqrt(
          double(x - center.x) * double(x - center.x)
          + double(y - center.y) * double(y - center.y)
          + double(z - center.z) * double(z - center.z)
        ));
        if (distanceBetween <= renderDistance) {
          positions.Add(ChunkPosition(x, y, z));
        }
      }
    }
  }

  return positions;
}
