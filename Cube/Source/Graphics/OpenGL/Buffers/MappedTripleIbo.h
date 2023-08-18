#pragma once

#include "../../../Engine/EngineCore.h"

class IndexBufferObject;

class MappedTripleIbo
{
public:

	struct Mapped {
		IndexBufferObject* buffer;
		uint32* data;

		Mapped() : buffer(nullptr), data(nullptr) {}
	};

	MappedTripleIbo();

	~MappedTripleIbo();

	void Reserve(uint32 count);

	void SwapNextBuffer();

	Mapped& GetBoundMapped() { return mappedBuffers[boundId]; }
	Mapped& GetModifyMapped() { return mappedBuffers[modifyId]; }
	uint32 GetCapacity() const { return capacity; }

	IndexBufferObject* GetBoundBuffer() { return mappedBuffers[boundId].buffer; }

private:

	void DeleteBuffers();

private:

	Mapped mappedBuffers[3];
	uint32 capacity;
	uint32 boundId;
	uint32 modifyId;
};