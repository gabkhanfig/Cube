#include "IndexBufferObject.h"
#include <glad/glad.h>
#include "../../../Engine/Engine.h"
#include "../../../Engine/OpenGL/OpenGLInstance.h"

uint32 IndexBufferObject::boundId = 0;

IndexBufferObject::IndexBufferObject()
	: indexCount(0)
{
	assertOnRenderThread();
	glCreateBuffers(1, &id);
}

//IndexBufferObject::IndexBufferObject(const uint32* indices, uint32 num)
//	: indexCount(num)
//{
//	assertOnRenderThread();
//	glCreateBuffers(1, &id);
//	Bind();
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * num, indices, GL_STATIC_DRAW);
//}

void IndexBufferObject::BufferData(const uint32* indices, uint32 num)
{
	assertOnRenderThread();
	glNamedBufferData(id, num * sizeof(uint32), indices, GL_STATIC_DRAW);
}

uint32* IndexBufferObject::CreatePersistentMappedStorage(uint32 elementCapacity)
{
	assertOnRenderThread();
	const GLbitfield mapFlags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
	const uint32 bufferCapacity = elementCapacity * sizeof(uint32);

	glNamedBufferStorage(id, bufferCapacity, nullptr, mapFlags);
	uint32* mappedBufferRange = (uint32*)glMapNamedBufferRange(id, 0, bufferCapacity, mapFlags);
	gk_assertNotNull(mappedBufferRange);
	return mappedBufferRange;
}

IndexBufferObject::~IndexBufferObject()
{
	assertOnRenderThread();
	if (IsBound()) {
		boundId = 0;
	}
	glDeleteBuffers(1, &id);
}

IndexBufferObject* IndexBufferObject::CreatePersistentMapped(uint32 capacity, void** mappedBufferOut)
{
	gk_assertm(mappedBufferOut, "mappedBufferOut must be a non-null pointer to copy the mapped buffer to");
	IndexBufferObject* ibo = new IndexBufferObject();
	ibo->Bind();
	GLbitfield mapFlags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
	glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, capacity, 0, mapFlags);
	void* bufferRange = glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, capacity, mapFlags);
	gk_assertm(bufferRange, "glMapBufferRange returned nullptr");
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


