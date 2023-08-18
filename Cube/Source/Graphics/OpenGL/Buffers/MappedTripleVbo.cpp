#include "MappedTripleVbo.h"
//#include "VertexBufferObject.h"
#include "../../../Engine/OpenGL/OpenGLInstance.h"
#include "../../../Engine/Engine.h"

/*
template<typename T>
MappedTripleVbo<T>::MappedTripleVbo()
	: capacity(0), boundId(0), modifyId(1) {}

template<typename T>
MappedTripleVbo<T>::~MappedTripleVbo()
{
	assertOnRenderThread();
	DeleteBuffers();
}

template<typename T>
void MappedTripleVbo<T>::Reserve(uint32 count)
{
	assertOnRenderThread();
	gk_assertm(count != 0, "Cannot reserve mapped triple index buffer objects with a capacity of 0");

	DeleteBuffers();
	capacity = count;
	boundId = 0;
	modifyId = 1;

	for (int i = 0; i < 3; i++) {
		Mapped mapped;
		mapped.buffer = new VertexBufferObject<T>();
		mapped.data = mapped.buffer->CreatePersistentMappedStorage(capacity);
		mappedBuffers[i] = mapped;
	}
}

template<typename T>
void MappedTripleVbo<T>::SwapNextBuffer()
{
	assertOnRenderThread();
	boundId = (boundId + 1) % 3;
	modifyId = (modifyId + 1) % 3;
}

template<typename T>
void MappedTripleVbo<T>::DeleteBuffers()
{
	assertOnRenderThread();
	for (int i = 0; i < 3; i++) {
		VertexBufferObject<T>* bufferObject = mappedBuffers[i].buffer;
		if (bufferObject) delete bufferObject;
		mappedBuffers[i] = Mapped();
	}
}*/
