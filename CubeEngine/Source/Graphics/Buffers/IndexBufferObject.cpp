#include "IndexBufferObject.h"

#include <glad/glad.h>

uint32 IndexBufferObject::boundId = 0;

IndexBufferObject::IndexBufferObject(const uint32* indices, uint32 num)
	: indexCount(num)
{
	glGenBuffers(1, &id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	boundId = id;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * num, indices, GL_STATIC_DRAW);
}

IndexBufferObject::~IndexBufferObject()
{
	if (IsBound()) {
		boundId = 0;
	}
	glDeleteBuffers(1, &id);
}

void IndexBufferObject::Bind()
{
	if (IsBound()) return;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	boundId = id;
}

void IndexBufferObject::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	boundId = 0;
}

bool IndexBufferObject::IsBound() const
{
	return id == boundId;
}


