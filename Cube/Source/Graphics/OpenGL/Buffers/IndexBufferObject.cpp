#include "IndexBufferObject.h"
#include <glad/glad.h>

uint32 IndexBufferObject::boundId = 0;

IndexBufferObject::IndexBufferObject()
	: indexCount(0)
{
	glGenBuffers(1, &id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	boundId = id;
}

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

IndexBufferObject* IndexBufferObject::CreatePersistentMapped(uint32 capacity, void** mappedBufferOut)
{
	checkm(mappedBufferOut, "mappedBufferOut must be a non-null pointer to copy the mapped buffer to");
	IndexBufferObject* ibo = new IndexBufferObject();
	GLbitfield mapFlags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
	glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, capacity, 0, mapFlags);
	void* bufferRange = glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, capacity, mapFlags);
	checkm(bufferRange, "glMapBufferRange returned nullptr");
	*mappedBufferOut = bufferRange;
	return ibo;
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

uint32* IndexBufferObject::GetMapBuffer()
{
	Bind();
	return (uint32*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
}

void IndexBufferObject::UnmapBuffer()
{
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
}


