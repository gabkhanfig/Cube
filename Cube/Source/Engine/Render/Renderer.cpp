#include "Renderer.h"
#include <Engine/Render/Buffers/BufferObjects.h>
#include <Engine/Render/Shader/Shader.h>
#include <Engine/Render/Geometry/Vertex.h>
#include <Engine/Render/Texture/Texture.h>
#include <Engine/Render/Geometry/Quad.h>
#include <Engine/Render/Camera/Camera.h>
#include <Game/World/Chunk/Chunk.h>

void Renderer::LoadGraphicsAPI()
{
#if RENDERER == OPEN_GL

  gladLoadGL();
  glClearColor(0.2, 0.4, 0.74, 0);
  glEnable(GL_DEPTH_TEST);

  std::string versionString = std::string((const char*)glGetString(GL_VERSION));
  Print("OpenGL Version: " + versionString);

  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(OpenGLDebugMessageCallback, 0);

  Shader* shader1 = Shader::LoadShader("Basic.vert", "Basic.frag");

  //Texture* tex = Texture::MakeTexture2d();
  //tex->SetTexture2dImage("Source/Resources/Textures/Blocks/stonetexture.png");
  //tex->Bind();

  Texture* texArray = Texture::MakeTexture2dArray(2048, 16, 16);
  texArray->AddTextureToArray("Source/Resources/Textures/Blocks/stonetexture.png");
  texArray->Bind();

  Camera* camera = new Camera();
  Camera::SetActiveCamera(camera);

#endif
}

void Renderer::DrawTriangles(uint32 triangleCount)
{
#if RENDERER == OPEN_GL
  glDrawElements(GL_TRIANGLES, 3 * triangleCount, GL_UNSIGNED_INT, nullptr);
#endif
}

void Renderer::DrawQuads(uint32 quadCount)
{
#if RENDERER == OPEN_GL
  glDrawElements(GL_TRIANGLES, 6 * quadCount, GL_UNSIGNED_INT, nullptr);
#endif
}

void Renderer::Clear()
{
#if RENDERER == OPEN_GL
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif
}

void Renderer::OpenGLDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
  std::string callbackPrefix = "[Open GL]: ";
  std::string callbackMessage = message;
  std::string _source;
  std::string _type;
  std::string _severity;

  switch (source) {
    case GL_DEBUG_SOURCE_API:
      _source = "API";
      break;

    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
      _source = "WINDOW SYSTEM";
      break;

    case GL_DEBUG_SOURCE_SHADER_COMPILER:
      _source = "SHADER COMPILER";
      break;

    case GL_DEBUG_SOURCE_THIRD_PARTY:
      _source = "THIRD PARTY";
      break;

    case GL_DEBUG_SOURCE_APPLICATION:
      _source = "APPLICATION";
      break;

    case GL_DEBUG_SOURCE_OTHER:
      _source = "UNKNOWN";
      break;

    default:
      _source = "UNKNOWN";
      break;
  }

  switch (type) {
    case GL_DEBUG_TYPE_ERROR:
      _type = "ERROR";
      break;

    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
      _type = "DEPRECATED BEHAVIOR";
      break;

    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
      _type = "UDEFINED BEHAVIOR";
      break;

    case GL_DEBUG_TYPE_PORTABILITY:
      _type = "PORTABILITY";
      break;

    case GL_DEBUG_TYPE_PERFORMANCE:
      _type = "PERFORMANCE";
      break;

    case GL_DEBUG_TYPE_OTHER:
      _type = "OTHER";
      break;

    case GL_DEBUG_TYPE_MARKER:
      _type = "MARKER";
      break;

    default:
      _type = "UNKNOWN";
      break;
  }

  switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
      _severity = "HIGH";
      break;

     case GL_DEBUG_SEVERITY_MEDIUM:
      _severity = "MEDIUM";
      break;

    case GL_DEBUG_SEVERITY_LOW:
      _severity = "LOW";
      break;

    case GL_DEBUG_SEVERITY_NOTIFICATION:
      _severity = "NOTIFICATION";
      break;

    default:
      _severity = "UNKNOWN";
      break;
  }

  if (type == GL_DEBUG_TYPE_ERROR) {
    Print(callbackPrefix + _type + " Debug message callback. \nId: " + ToString(id) + "\nSeverity: " + _severity + "\nSource: " + _source + "\nMessage: " + callbackMessage);
  }
}

void Renderer::Frame(double deltaTime)
{

  //Shader* shader2 = Shader::LoadShader("Compressed.vert", "Basic.frag");
  Shader::GetBoundShader()->SetUniformMat4float("u_MVP", Camera::activeCamera->GetMVPMatrix());

  //shader1->Bind();

  glm::vec3 pos[] = {
    {-0.5, -0.5, 0},
    {0.5, -0.5, 0},
    {0.5, 0.5, 0},
    {-0.5, 0.5, 0}
  };

  glm::vec4 col[] = {
    {1.0, 1.0, 0.2, 1},
    {1.0, 0.5, 0.2, 1},
    {0.7, 0.1, 0.5, 1},
    {0, 0, 1, 1}
  };

  Quad quad;
  Quad::GenerateQuad1x1(quad, pos, col, 0);

  VertexBuffer* vbo = new VertexBuffer(&quad, sizeof(quad));

  VertexArray* vao = new VertexArray();
  vao->AddBuffer(vbo, VertexBufferLayout::MakeVertexBufferLayoutForVertex());

  IndexBuffer* ibo = IndexBuffer::MakeQuadsIndexBuffer(1);

  
  //DrawQuads(1);

  delete vbo;
  delete vao;
  delete ibo;

  /*
  shader2->Bind();

  BlockVertexPosition pos2[] = {
    {0, 0, 0, BlockVertexPosition::bottomLeftUV},
    {1, 0, 0, BlockVertexPosition::bottomRightUV},
    {1, 1, 0, BlockVertexPosition::topRightUV},
    {0, 1, 0, BlockVertexPosition::topLeftUV}
  };

  Color col2[] = {
    {255, 205, 55, 255},
    {10, 170, 190, 255},
    {120, 110, 60, 255},
    {255, 78, 255, 255}
  };

  BlockVertex a = { pos2[0], col2[0]};
  BlockVertex b = { pos2[1], col2[1] };
  BlockVertex c = { pos2[2], col2[2] };
  BlockVertex d = { pos2[3], col2[3] };

  BlockQuad quad2 = {
    a, b, c, d
  };
  //BlockQuad::GenerateQuad1x1(quad2, pos2, col2, 0);

  VertexBuffer* vbo2 = new VertexBuffer(&quad2, sizeof(quad2));

  VertexArray* vao2 = new VertexArray();
  vao2->AddBuffer(vbo2, VertexBufferLayout::MakeVertexBufferLayoutForBlock());

  IndexBuffer* ibo2 = IndexBuffer::MakeQuadsIndexBuffer(1);

  DrawQuads(1);

  delete vbo2;
  delete vao2;
  delete ibo2;

  //
  delete shader2;
  */

  Chunk* testDrawTime = new Chunk({ 1, 0, 0 });
  testDrawTime->DrawChunk();
  delete testDrawTime;
}
