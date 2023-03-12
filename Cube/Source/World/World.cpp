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

void World::TestFirstChunkRemesh()
{
  auto start1 = std::chrono::high_resolution_clock::now();
  testChunk = new Chunk();
  chunks.insert({ testChunk->GetPosition(), testChunk });
  testChunk->FillChunkWithBlock("stoneBlock");
  auto stop1 = std::chrono::high_resolution_clock::now();
  auto start2 = std::chrono::high_resolution_clock::now();
  testChunk->RecreateMesh();
  auto stop2 = std::chrono::high_resolution_clock::now();
  auto start3 = std::chrono::high_resolution_clock::now();
  testChunk->GetRenderComponent()->MeshToOpenGLObjects();
  auto stop3 = std::chrono::high_resolution_clock::now();
  std::cout << "Time to create chunk mesh on the CPU:  " << std::chrono::duration_cast<std::chrono::milliseconds>(stop2 - start2).count() << "ms\n";
  std::cout << "Time to send to chunk mesh to the GPU: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop3 - start3).count() << "ms\n";

}

void World::DrawWorld()
{
  /* Render here */
  Renderer::Clear();

  //chunkShader->Bind();
  Camera* cam = Camera::GetActiveCamera();
  //chunkShader->SetUniformMat4f("u_cameraMVP", cam->GetMvpMatrix());
  //chunkShader->SetUniform3f("u_chunkOffset", { 10, 10, 10 });
  chunkRenderer->SetShaderCameraMVP(cam->GetMvpMatrix());
  //chunkRenderer->SetShaderChunkOffset({ 10, 10, 10 });
  chunkRenderer->SetShaderChunkOffset(chunkRenderer->GetOffsetForChunkDraw(testChunk));

  testChunk->RecreateMesh();
  //testChunk->GetRenderComponent()->MeshToOpenGLObjects();
  //testChunk->GetRenderComponent()->CopyToSameVBO();
  testChunk->Draw(chunkRenderer);
}
