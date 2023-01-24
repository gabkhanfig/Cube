#include "ShaderBufferObject.h"
#include <glad/glad.h>

ShaderBufferObject::ShaderBufferObject(const void* data, uint32 size, uint32 index)
{
	glGenBuffers(1, &id);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
  glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_STATIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, id);
}

ShaderBufferObject::~ShaderBufferObject()
{
  glDeleteBuffers(1, &id);
}

void ShaderBufferObject::Bind()
{
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
}

void ShaderBufferObject::Unbind()
{
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void* ShaderBufferObject::GetMapBuffer()
{
  Bind();
  return glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
}

void ShaderBufferObject::UnmapBuffer()
{
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}
