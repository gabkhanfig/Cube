#include "MappedTripleIbo.h"
#include "IndexBufferObject.h"
#include "../../../Engine/OpenGL/OpenGLInstance.h"
#include "../../../Engine/Engine.h"

MappedTripleIbo::MappedTripleIbo() : capacity(0), boundId(0), modifyId(1) {}

MappedTripleIbo::~MappedTripleIbo()
{
	assertOnRenderThread();
	DeleteBuffers();
}

void MappedTripleIbo::Reserve(uint32 count)
{
	assertOnRenderThread();
	gk_assertm(count != 0, "Cannot reserve mapped triple index buffer objects with a capacity of 0");

	DeleteBuffers();
	capacity = count;
	boundId = 0;
	modifyId = 1;

	for (int i = 0; i < 3; i++) {
		Mapped mapped;
		mapped.buffer = new IndexBufferObject();
		mapped.data = mapped.buffer->CreatePersistentMappedStorage(capacity);
		mappedBuffers[i] = mapped;
	}
}

void MappedTripleIbo::SwapNextBuffer()
{
	boundId = (boundId + 1) % 3;
	modifyId = (modifyId + 1) % 3;
}

void MappedTripleIbo::DeleteBuffers()
{
	assertOnRenderThread();
	for (int i = 0; i < 3; i++) {
		IndexBufferObject* bufferObject = mappedBuffers[i].buffer;
		if (bufferObject) delete bufferObject;
		mappedBuffers[i] = Mapped();
	}
}
