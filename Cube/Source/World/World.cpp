#include "World.h"
#include "../GameInstance.h"
#include "../Player/Player.h"
#include <Graphics/Camera/Camera.h>
#include <Graphics/Shader/Shader.h>
#include <Graphics/Buffers/VertexBufferObject.h>
#include <Graphics/Buffers/IndexBufferObject.h>
#include <Graphics/Buffers/VertexArrayObject.h>
#include <Graphics/Buffers/ShaderBufferObject.h>
#include "../../Resources/GeneratedFiles.h"
#include <Graphics/Geometry/BlockGeometry.h>
#include <glad/glad.h>
#include "Block/BlockTextureAtlas.h"

World* GetWorld()
{
  return GetGameInstance()->GetWorld();
}

World::World()
{
  player = new Player();
  chunkShader = new Shader(generated_Chunk_vert, generated_Chunk_frag);

  //BlockTextureAtlas::Bind(0);
}

void World::DrawWorld()
{
  /* Render here */
  glClear(GL_COLOR_BUFFER_BIT);

  chunkShader->Bind();

  Camera* cam = Camera::GetActiveCamera();
  chunkShader->SetUniformMat4f("u_cameraMVP", cam->GetMvpMatrix());

  uint32 positions[4] = {
    {255 + (255 << 8)},
    {255},
    {0}, //  + (255 << 16)
    {(255 << 8)}
  };

  glm::vec2 texCoords[4] = {
    BlockTextureAtlas::GetTextureCoord(BlockTexture::wood, {1, 1}),
    BlockTextureAtlas::GetTextureCoord(BlockTexture::wood, {1, 0}),
    BlockTextureAtlas::GetTextureCoord(BlockTexture::wood, {0, 0}),
    BlockTextureAtlas::GetTextureCoord(BlockTexture::wood, {0, 1})
  };

  IndexBufferObject* ibo = BlockQuad::CreateQuadsIndexBuffer(1);
  BlockQuad quad = BlockQuad(positions, texCoords);
  VertexArrayObject* vao = new VertexArrayObject();
  vao->Bind();
  VertexBufferObject* vbo = BlockQuad::CreateQuadsVertexBufferObject(&quad, 1);
  vao->LinkVertexBufferObject(vbo, BlockQuad::GetQuadsVertexBufferLayout());

  

  vao->Bind();
  ibo->Bind();
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  delete vao;
  delete vbo;
  delete ibo;
}
