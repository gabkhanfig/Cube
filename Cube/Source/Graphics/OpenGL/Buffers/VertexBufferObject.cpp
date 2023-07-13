#include "VertexBufferObject.h"
#include <glad/glad.h>

uint32 VertexBufferObject::boundId = 0;

VertexBufferObject::VertexBufferObject()
{
	glGenBuffers(1, &id);
	Bind();
	gk_assert(IsBound());
}

VertexBufferObject::VertexBufferObject(const void* data, uint32 size)
{
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	boundId = id;
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBufferObject::~VertexBufferObject()
{
	if (IsBound()) {
		boundId = 0;
	}
	glDeleteBuffers(1, &id);
}

VertexBufferObject* VertexBufferObject::CreatePersistentMapped(uint32 capacity, void** mappedBufferOut)
{
	gk_assertm(mappedBufferOut, "mappedBufferOut must be a non-null pointer to copy the mapped buffer to");
	VertexBufferObject* vbo = new VertexBufferObject();
	GLbitfield mapFlags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
	glBufferStorage(GL_ARRAY_BUFFER, capacity, 0, mapFlags);
	void* bufferRange = glMapBufferRange(GL_ARRAY_BUFFER, 0, capacity, mapFlags);
	gk_assertm(bufferRange, "glMapBufferRange returned nullptr");
	*mappedBufferOut = bufferRange;
	return vbo;
}

void VertexBufferObject::Bind()
{
	if (IsBound()) return;
	glBindBuffer(GL_ARRAY_BUFFER, id);
	boundId = id;
}

void VertexBufferObject::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	boundId = 0;
}

bool VertexBufferObject::IsBound() const
{
	return id == boundId;
}

void* VertexBufferObject::GetMapBuffer()
{
	Bind();
	return glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
}

void VertexBufferObject::UnmapBuffer()
{
	glUnmapBuffer(GL_ARRAY_BUFFER);
}


