#pragma once

#include "../../../Engine/EngineCore.h"
#include "../OpenGLStructures.h"

class DrawIndirectBufferObject;

class MappedTripleDibo
{
public:

	struct Mapped {
		DrawIndirectBufferObject* buffer;
		DrawElementsIndirectCommand* data;

		Mapped() : buffer(nullptr), data(nullptr) {}
	};

	MappedTripleDibo();

	~MappedTripleDibo();

	void Reserve(uint32 count);

	void SwapNextBuffer();

	Mapped& GetBoundMapped() { return mappedBuffers[boundId]; }
	Mapped& GetModifyMapped() { return mappedBuffers[modifyId]; }
	uint32 GetCapacity() const { return capacity; }

	DrawIndirectBufferObject* GetBoundBuffer() { return mappedBuffers[boundId].buffer; }

private:

	void DeleteBuffers();

private:

	Mapped mappedBuffers[3];
	uint32 capacity;
	uint32 boundId;
	uint32 modifyId;
};