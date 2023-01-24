#include "IndexBufferObject.h"

#include <glad/glad.h>

IndexBufferObject::IndexBufferObject(const uint32* indices, uint32 num)
{
	glGenBuffers(1, &id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * num, indices, GL_STATIC_DRAW);
}

IndexBufferObject::~IndexBufferObject()
{
	glDeleteBuffers(1, &id);
}

void IndexBufferObject::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void IndexBufferObject::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


