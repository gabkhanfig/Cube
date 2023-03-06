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

#include <chrono>

World* GetWorld()
{
  return GetGameInstance()->GetWorld();
}

World::World()
{
  player = new Player();
  chunkShader = new Shader(generated_Chunk_vert, generated_Chunk_frag);
  chunkVAO = new VertexArrayObject();
  chunkVAO->SetFormatLayout(BlockQuad::GetQuadsVertexBufferLayout());

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
  std::cout << "Time to create and fill chunk: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop1 - start1).count() << "ms\n";
  std::cout << "Time to create chunk mesh: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop2 - start2).count() << "ms\n";
  std::cout << "Time to send to GPU: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop3 - start3).count() << "ms\n";

  //BlockTextureAtlas::Bind(0);
}

void World::DrawWorld()
{
  /* Render here */
  Renderer::Clear();

  chunkShader->Bind();
  Camera* cam = Camera::GetActiveCamera();
  chunkShader->SetUniformMat4f("u_cameraMVP", cam->GetMvpMatrix());

  //glm::vec3 positions[4] = {
  //  glm::vec3(0, 0, 1),
  //  glm::vec3(1, 0, 1),
  //  glm::vec3(1, 1, 1),
  //  glm::vec3(0, 1, 1)
  //};

  //glm::vec2 texCoords[4] = {
  //  BlockTextureAtlas::GetTextureCoord(EBlockTexture::stone, {0, 0}),
  //  BlockTextureAtlas::GetTextureCoord(EBlockTexture::stone, {1, 0}),
  //  BlockTextureAtlas::GetTextureCoord(EBlockTexture::stone, {1, 1}),
  //  BlockTextureAtlas::GetTextureCoord(EBlockTexture::stone, {0, 1})
  //};

  //IndexBufferObject* ibo = BlockQuad::CreateQuadsIndexBuffer(1);
  //BlockQuad quad = BlockQuad(positions, texCoords);
  //VertexBufferObject* vbo = BlockQuad::CreateQuadsVertexBufferObject(&quad, 1);

  //chunkVAO->BindVertexBufferObject(vbo, sizeof(BlockVertex));

  //Renderer::DrawVboTriangles(vbo, ibo);

  //glm::vec3 positions2[4] = {
  //  glm::vec3(1, 1, 1),
  //  glm::vec3(2, 1, 1),
  //  glm::vec3(2, 2, 1),
  //  glm::vec3(1, 2, 1)
  //};

  //glm::vec2 texCoords2[4] = {
  //  BlockTextureAtlas::GetTextureCoord(EBlockTexture::stone, {0, 0}),
  //  BlockTextureAtlas::GetTextureCoord(EBlockTexture::stone, {1, 0}),
  //  BlockTextureAtlas::GetTextureCoord(EBlockTexture::stone, {1, 1}),
  //  BlockTextureAtlas::GetTextureCoord(EBlockTexture::stone, {0, 1})
  //};

  //BlockQuad quad2 = BlockQuad(positions2, texCoords2);
  //VertexBufferObject* vbo2 = BlockQuad::CreateQuadsVertexBufferObject(&quad2, 1);

  //chunkVAO->BindVertexBufferObject(vbo2, sizeof(BlockVertex));
  //Renderer::DrawVboTriangles(vbo2, ibo);

  /*IBlock* stone = BlockFactory::GetBlockClass("stoneBlock")->GetBlock();
  ChunkMesh mesh;
  mesh.AddBlockMesh(stone->CreateBlockMesh(nullptr, { 0, 0, 0 }));
  VertexBufferObject* stoneVBO = mesh.MakeVertexBufferObject();
  IndexBufferObject* stoneIBO = mesh.MakeIndexBufferObject();
  chunkVAO->BindVertexBufferObject(stoneVBO, sizeof(BlockVertex));
  Renderer::DrawVboTriangles(stoneVBO, stoneIBO);*/

  testChunk->Draw(chunkShader, chunkVAO);
  //delete vao;
  //delete vbo;
 // delete ibo;
  //delete vbo2;
  //delete stoneVBO;
  //delete stoneIBO;
}
