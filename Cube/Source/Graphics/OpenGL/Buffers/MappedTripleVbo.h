#pragma once

#include "../../../Engine/EngineCore.h"
#include "../../../Graphics/OpenGL/Buffers/VertexBufferObject.h"
#include "../../../Engine/OpenGL/OpenGLInstance.h"
#include "../../../Engine/Engine.h"

//template<typename T>
//class VertexBufferObject;

template<typename T>
class MappedTripleVbo
{
public:

	struct Mapped {
		VertexBufferObject* buffer;
		T* data;

		Mapped() : buffer(nullptr), data(nullptr) {}
	};

	MappedTripleVbo() 
		: capacity(0), boundId(0), modifyId(1) {}

	~MappedTripleVbo() {
		assertOnRenderThread();
		DeleteBuffers();
	}

	void Reserve(uint32 count) {
		assertOnRenderThread();
		gk_assertm(count != 0, "Cannot reserve mapped triple index buffer objects with a capacity of 0");

		DeleteBuffers();
		capacity = count;
		boundId = 0;
		modifyId = 1;

		for (int i = 0; i < 3; i++) {
			Mapped mapped;
			mapped.buffer = new VertexBufferObject();
			mapped.data = mapped.buffer->CreatePersistentMappedStorage<T>(capacity);
			mappedBuffers[i] = mapped;
		}
	}

	void SwapNextBuffer() {
		assertOnRenderThread();
		boundId = (boundId + 1) % 3;
		modifyId = (modifyId + 1) % 3;
	}

	Mapped& GetBoundMapped() { return mappedBuffers[boundId]; }
	Mapped& GetModifyMapped() { return mappedBuffers[modifyId]; }
	uint32 GetCapacity() const { return capacity; }

	VertexBufferObject* GetBoundBuffer() { return mappedBuffers[boundId].buffer; }

private:

	void DeleteBuffers() {
		assertOnRenderThread();
		for (int i = 0; i < 3; i++) {
			VertexBufferObject* bufferObject = mappedBuffers[i].buffer;
			if (bufferObject) delete bufferObject;
			mappedBuffers[i] = Mapped();
		}
	}

private:

	Mapped mappedBuffers[3];
	uint32 capacity;
	uint32 boundId;
	uint32 modifyId;
};


