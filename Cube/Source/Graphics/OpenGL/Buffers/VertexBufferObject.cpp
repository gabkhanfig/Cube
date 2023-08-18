#include "VertexBufferObject.h"
#include <glad/glad.h>
#include "../../../Engine/Engine.h"
#include "../../../Engine/OpenGL/OpenGLInstance.h"

uint32 VertexBufferObject::boundId = 0;

VertexBufferObject::VertexBufferObject()
{
	assertOnRenderThread();
	glCreateBuffers(1, &id);
}

VertexBufferObject::~VertexBufferObject()
{
	assertOnRenderThread();
	if (IsBound()) {
		boundId = 0;
	}
	glDeleteBuffers(1, &id);
}

//void VertexBufferObject::BufferData(const T* data, uint32 elementCount)
//{
//	assertOnRenderThread();
//	glNamedBufferData(id, elementCount * sizeof(T), data, GL_STATIC_DRAW);
//}

//T* VertexBufferObject::CreatePersistentMappedStorage(uint32 elementCapacity)
//{
//	assertOnRenderThread();
//	const GLbitfield mapFlags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
//	const uint32 bufferCapacity = elementCapacity * sizeof(T);
//
//	glNamedBufferStorage(id, bufferCapacity, nullptr, mapFlags);
//	T* mappedBufferRange = (T*)glMapNamedBufferRange(id, 0, bufferCapacity, mapFlags);
//	gk_assertNotNull(mappedBufferRange);
//	return mappedBufferRange;
//}

void VertexBufferObject::Bind()
{
	assertOnRenderThread();
	if (IsBound()) return;
	glBindBuffer(GL_ARRAY_BUFFER, id);
	boundId = id;
}

void VertexBufferObject::Unbind()
{
	assertOnRenderThread();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	boundId = 0;
}

bool VertexBufferObject::IsBound() const
{
	return id == boundId;
}

void VertexBufferObject::UnmapBuffer()
{
	assertOnRenderThread();
	GLboolean result = glUnmapNamedBuffer(id);
	gk_assertm(result != GL_FALSE, "Error with unmapping vbo buffer");
}

void VertexBufferObject::BufferDataImpl(const void* data, uint32 totalBytes)
{
	assertOnRenderThread();
	glNamedBufferData(id, totalBytes, data, GL_STATIC_DRAW);
}

void* VertexBufferObject::CreatePersistentMappedStorageImpl(const uint32 totalByteCapacity)
{
	assertOnRenderThread();
	const GLbitfield mapFlags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;

	glNamedBufferStorage(id, totalByteCapacity, nullptr, mapFlags);
	void* mappedBufferRange = glMapNamedBufferRange(id, 0, totalByteCapacity, mapFlags);
	gk_assertNotNull(mappedBufferRange);
	return mappedBufferRange;
}

void* VertexBufferObject::GetMapBufferImpl(GLMappedBufferAccess access)
{
	assertOnRenderThread();
	void* mappedBuffer = glMapNamedBuffer(id, static_cast<int>(access));
	gk_assertNotNull(mappedBuffer);
	return mappedBuffer;
}

//VertexBufferObject::VertexBufferObject()
//{
//	assertOnRenderThread();
//	glCreateBuffers(1, &id);
//}
//
//VertexBufferObject::VertexBufferObject(const void* data, uint32 size)
//{
//	assertOnRenderThread();
//	glCreateBuffers(1, &id);
//	Bind();
//	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
//}
//
//VertexBufferObject::~VertexBufferObject()
//{
//	assertOnRenderThread();
//	if (IsBound()) {
//		boundId = 0;
//	}
//	glDeleteBuffers(1, &id);
//}
//
//VertexBufferObject* VertexBufferObject::CreatePersistentMapped(uint32 capacity, void** mappedBufferOut)
//{
//	gk_assertm(mappedBufferOut, "mappedBufferOut must be a non-null pointer to copy the mapped buffer to");
//	VertexBufferObject* vbo = new VertexBufferObject();
//	vbo->Bind();
//	GLbitfield mapFlags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
//	glBufferStorage(GL_ARRAY_BUFFER, capacity, 0, mapFlags);
//	void* bufferRange = glMapBufferRange(GL_ARRAY_BUFFER, 0, capacity, mapFlags);
//	gk_assertm(bufferRange, "glMapBufferRange returned nullptr");
//	*mappedBufferOut = bufferRange;
//	return vbo;
//}
//
//void VertexBufferObject::Bind()
//{
//	if (IsBound()) return;
//	glBindBuffer(GL_ARRAY_BUFFER, id);
//	boundId = id;
//}
//
//void VertexBufferObject::Unbind()
//{
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	boundId = 0;
//}
//
//bool VertexBufferObject::IsBound() const
//{
//	return id == boundId;
//}
//
//void* VertexBufferObject::GetMapBuffer()
//{
//	Bind();
//	return glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
//}
//
//void VertexBufferObject::UnmapBuffer()
//{
//	glUnmapBuffer(GL_ARRAY_BUFFER);
//}

