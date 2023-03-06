#include "VertexBufferObject.h"
#include <glad/glad.h>

uint32 VertexBufferObject::boundId = 0;

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


