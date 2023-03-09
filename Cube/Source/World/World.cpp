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
  return GetGameInstance()->GetWorld();
}

World::World()
{
  player = new Player();
  chunkRenderer = new ChunkRenderer();
  //chunkShader = new Shader(generated_Chunk_vert, generated_Chunk_frag);
  //chunkVAO = new VertexArrayObject();
  //chunkVAO->SetFormatLayout(BlockQuad::GetQuadsVertexBufferLayout());

  auto start1 = std::chrono::high_resolution_clock::now();
  testChunk = new Chunk();
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

  //BlockTextureAtlas::Bind(0);
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
  testChunk->GetRenderComponent()->CopyToSameVBO();
  testChunk->Draw(chunkRenderer);
}
