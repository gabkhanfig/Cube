#pragma once

#include "../../../Engine/EngineCore.h"

template<typename Buffer, typename BufferDataType>
class PersistentMappedTripleBuffer 
{
public:

	struct Mapped {
		Buffer* buffer;
		BufferDataType* data;

		Mapped() : buffer(nullptr), data(nullptr) {}
	};

	PersistentMappedTripleBuffer() : capacity(0), boundId(0), modifyId(1) {}

	~PersistentMappedTripleBuffer() {
		DeleteBuffers();
	}

	void Reserve(uint32 count) {
		DeleteBuffers();
		capacity = count;
		boundId = 0;
		modifyId = 1;

		for (int i = 0; i < 3; i++) {
			Mapped mapped;
			mapped.buffer = Buffer::CreatePersistentMapped(capacity * sizeof(BufferDataType), (void**)&mapped.data);
			mappedBuffers[i] = mapped;
		}
	}

	/* Swap the bound and modify buffers to the next one, round robin. */
	void SwapNextBuffer() {
		boundId = (boundId + 1) % 3;
		modifyId = (modifyId + 1) % 3;
	}

	Mapped& GetBoundMapped() { return mappedBuffers[boundId]; }
	Mapped& GetModifyMapped() { return mappedBuffers[modifyId]; }
	uint32 GetCapacity() const { return capacity; }

	Buffer* GetBoundBuffer() const { return mappedBuffers[boundId].buffer; }

private:

	void DeleteBuffers() {
		for (int i = 0; i < 3; i++) {
			Buffer* bufferObject = mappedBuffers[i].buffer;
			if (bufferObject) delete bufferObject;
			mappedBuffers[i] = Mapped();
		}
	}


	Mapped mappedBuffers[3];
	uint32 capacity;
	uint32 boundId;
	uint32 modifyId;
};