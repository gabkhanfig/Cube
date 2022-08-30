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

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  std::string versionString = std::string((const char*)glGetString(GL_VERSION));
  Print("OpenGL Version: " + versionString);

  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(OpenGLDebugMessageCallback, 0);

  Shader* shader1 = Shader::LoadShader("Compressed.vert", "Compressed.frag");
  //shader1->SetUniform1int("u_UseTextureArray", true);

  Texture* tex = Texture::MakeTexture2d();
  tex->SetTexture2dImage("Source/Resources/Textures/Blocks/texture_atlas.png");
  float texRun = tex->GetWidth();
  shader1->SetUniform1float("u_AtlasSize", texRun);
  shader1->SetUniform1float("u_TextureSize", 16.f);

  //Texture* texArray = Texture::MakeTexture2dArray(2048, 16, 16);
  //texArray->AddTextureToArray("Source/Resources/Textures/Blocks/stonetexture.png");
  //texArray->Bind();

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
  Shader::GetBoundShader()->SetUniformMat4float("u_MVP", Camera::activeCamera->GetMVPMatrix());

  //Chunk* chunk = new Chunk({ 0, 0, 0 });
  //chunk->DrawChunk();
  //delete chunk;
}
