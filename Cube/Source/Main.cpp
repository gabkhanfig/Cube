#include <glad/glad.h>
#include <Engine.h>
#include <Window/Window.h>
#include <Graphics/Shader/Shader.h>
#include <Graphics/Buffers/VertexBufferObject.h>
#include <Graphics/Buffers/IndexBufferObject.h>
#include <Graphics/Buffers/VertexArrayObject.h>
#include <Graphics/Buffers/ShaderBufferObject.h>
#include "../Resources/GeneratedFiles.h"
#include <algorithm>
#include <Graphics/Geometry/BlockGeometry.h>
#include <Graphics/Texture/Texture2d.h>
#include <Input/UserInput.h>
#include <GLFW/glfw3.h>

#include "World/Block/Block.h"
#include "Vendor/stb_image/stb_image.h"
#include "World/Block/BlockTextureAtlas.h"

int main(void)
{
  Engine::Start();
  glClearColor(0.1, 0.1, 0.1, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  Shader* shader = new Shader(generated_Chunk_vert, generated_Chunk_frag);

  BlockTextureAtlas::LoadBlockAtlasTexture();

  IndexBufferObject* ibo = BlockQuad::CreateQuadsIndexBuffer(1);

  glm::vec4 ssbocolor = { 0, 0, 1, 1 };
  ShaderBufferObject* ssbo = ShaderBufferObject::Create(&ssbocolor, 1, 4);
  ShaderBufferObject* ssbo2 = ShaderBufferObject::Create(&ssbocolor, 1, 4);

  //void* ptr = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
  ssbocolor = { 1, 1, 0, 1 };
  memcpy(ssbo->GetMapBuffer(), &ssbocolor, sizeof(ssbocolor));
  //glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
  ssbocolor = { 1, 0, 1, 1 };
  memcpy(ssbo2->GetMapBuffer(), &ssbocolor, sizeof(ssbocolor));
  //glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

  /* Loop until the user closes the window */
  while (!engine->GetWindow()->ShouldWindowClose())
  {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);
    //uint32 positions[4] = {
    //  {255 + (255 << 8)},
    //  {255 },
    //  {0}, //  + (255 << 16)
    //  {(255 << 8)}
    //};

    uint32 positions[4] = {
      {255 + (255 << 8) + (255 << 16)},
      {255 + (255 << 16)},
      {0 + (255 << 16)}, //  + (255 << 16)
      {(255 << 8) + (255 << 16)}
    };

    glm::vec2 texCoords[4] = {
      BlockTextureAtlas::GetTextureCoord(BlockTexture::wood, {1, 1}),
      BlockTextureAtlas::GetTextureCoord(BlockTexture::wood, {1, 0}),
      BlockTextureAtlas::GetTextureCoord(BlockTexture::wood, {0, 0}),
      BlockTextureAtlas::GetTextureCoord(BlockTexture::wood, {0, 1})
    };
    //glm::vec2 texCoords[4] = {
    //  {1, 1},
    //  {1, 0},
    //  {0, 0},
    //  {0, 1}
    //};

    BlockQuad quad = BlockQuad(positions, texCoords);
    VertexArrayObject* vao = new VertexArrayObject();
    vao->Bind();
    VertexBufferObject* vbo = BlockQuad::CreateQuadsVertexBufferObject(&quad, 1);
    vao->LinkVertexBufferObject(vbo, BlockQuad::GetQuadsVertexBufferLayout());

    shader->Bind();

    vao->Bind();
    ibo->Bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    delete vao;
    delete vbo;

    /* Swap front and back buffers */
    engine->GetWindow()->SwapBuffers();

    /* Poll for and process events */
    Window::PollEvents();

    Sleep(50);
  }

  Window::TerminateGLFW();
}