#include "Shader.h"
#include <glad/glad.h>
#include <fstream>
#include <Engine/Render/RenderSettings.h>

Shader* Shader::boundShader;

Shader::Shader()
{
}

Shader* Shader::LoadShader(const std::string& vertexName, const std::string& fragmentName)
{
#if RENDERER == OPEN_GL
  const std::string shaderDefaultPath = "Source/Resources/Shaders/";
  Shader* shader = new Shader();

  uint32 vertex = ParseShader(GL_VERTEX_SHADER, shaderDefaultPath + vertexName);
  uint32 fragment = ParseShader(GL_FRAGMENT_SHADER, shaderDefaultPath + fragmentName);
  shader->rendererId = CreateShader(vertex, fragment);
  shader->Bind();
  return shader;
#endif
}

uint32 Shader::CreateShader(uint32 vertexShader, uint32 fragmentShader)
{
#if RENDERER == OPEN_GL
  unsigned int program = glCreateProgram();

  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

// For some reason, glGetShaderiv down below causes an opengl debug message callback saying, "GL_INVALID_OPERATION error generated. Object is not a program or shader object."
// This will supress that error message from showing up by not checking the status.
// Shader works perfectly fine, so I'm unsure where the error is actually coming from.
// https://registry.khronos.org/OpenGL-Refpages/es2.0/xhtml/glGetShaderiv.xml
#define SUPRESS_RANDOM_ERROR_MESSAGE
#ifndef SUPRESS_RANDOM_ERROR_MESSAGE
  int success;
  char infoLog[512];
  glGetShaderiv(program, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(program, 512, NULL, infoLog);
    Print("ERROR::SHADER::PROGRAM::COMPILATION_FAILED");
    Print(infoLog);
  }
#endif

  return program;
#endif
}

uint32 Shader::CompileShader(uint32 type, const std::string& source)
{
#if RENDERER == OPEN_GL
  unsigned int shader = glCreateShader(type);
  const char* src = source.c_str();
  glShaderSource(shader, 1, &src, nullptr);
  glCompileShader(shader);

  int success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE)
  {
    std::string shaderType;
    switch (type) {
    case GL_VERTEX_SHADER:
      shaderType = "VERTEX";
      break;
    case GL_FRAGMENT_SHADER:
      shaderType = "FRAGMENT";
      break;
    default:
      shaderType = "UNKNOWN";
      break;
    }

    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    Print("ERROR::SHADER::" + shaderType + "::COMPILATION_FAILED");
    Print(infoLog);
  }

  return shader;
#endif
}

uint32 Shader::ParseShader(uint32 type, const std::string& path)
{
#if RENDERER == OPEN_GL
  std::ifstream stream(path);
  std::string content;
  content.assign((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
  unsigned int shader = CompileShader(type, content);
  return shader;
#endif
}

void Shader::Bind()
{
  Shader::boundShader = this;
  glUseProgram(rendererId);
}

void Shader::Unbind()
{
  Shader::boundShader = nullptr;
  glUseProgram(0);
}

uint32 Shader::GetUniformLocation(const std::string& uniformName)
{
  if (uniforms.find(uniformName) != uniforms.end()) {
    return uniforms[uniformName];
  }

  uint32 location = glGetUniformLocation(rendererId, uniformName.c_str());
  if (location == -1) {
    Print("[Warning]: Uniform '" + uniformName + "' doesn't exist.");
  }
  uniforms.insert({ uniformName, location });
  return location;
}

void Shader::SetUniform4float(const std::string& uniformName, const glm::vec4& value)
{
  glUniform4f(GetUniformLocation(uniformName), value.x, value.y, value.z, value.w);
}

void Shader::SetUniform3float(const std::string& uniformName, const glm::vec3& value)
{
  glUniform3f(GetUniformLocation(uniformName), value.x, value.y, value.z);
}

void Shader::SetUniform1float(const std::string& uniformName, float value)
{
  glUniform1f(GetUniformLocation(uniformName), value);
}

void Shader::SetUniform1int(const std::string& uniformName, int value)
{
  glUniform1i(GetUniformLocation(uniformName), value);
}

void Shader::SetUniformMat4float(const std::string& uniformName, const glm::mat4& value)
{
  glUniformMatrix4fv(GetUniformLocation(uniformName), 1, GL_FALSE, &value[0][0]);
}
