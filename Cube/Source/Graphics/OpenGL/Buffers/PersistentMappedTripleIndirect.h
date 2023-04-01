#pragma once

#include "DrawIndirectBufferObject.h"

class PersistentMappedTripleIndirect
{
public:

	struct MappedIndirect
	{
		DrawIndirectBufferObject* dibo;
		DrawElementsIndirectCommand* data;
		uint32 size;

		MappedIndirect()
			: dibo(nullptr), data(nullptr), size(0)
		{}
	};

	PersistentMappedTripleIndirect();

	~PersistentMappedTripleIndirect();

	/* Delete (if they exist) the previous dibos and create 3 new dibos that have a maximum uint32 capacity of count. */
	void Reserve(uint32 count);

	/* Swap the bound and modify buffers to the next one, round robin. */
	void SwapNextBuffer();

	/* Get the mapped dibo data for the bound vbo. See SwapNextBuffer(). */
	MappedIndirect& GetBoundMappedDibo() { return dibos[boundId]; }
	/* Get the mapped dibo data for the one that will be swapped in next. See SwapNextBuffer(). */
	MappedIndirect& GetModifyMappedDibo() { return dibos[modifyId]; }
	/* Get the amount of uint32 elements that the ibos can hold. */
	uint32 GetCapacity() const { return capacity; }

	DrawIndirectBufferObject* GetBoundBufferObject() const { return dibos[boundId].dibo; }

private:

	void DeleteIndirectBuffers();

private:

	MappedIndirect dibos[3];
	uint32 capacity;
	int boundId;
	int modifyId;
};