#pragma once

#include "IndexBufferObject.h"

class PersistentMappedTripleIbo
{
public:

	struct MappedIbo {
		IndexBufferObject* ibo;
		uint32* data;
		uint32 size;

		MappedIbo()
			: ibo(nullptr), data(nullptr), size(0)
		{}
	};

	PersistentMappedTripleIbo();

	~PersistentMappedTripleIbo();

	/* Delete (if they exist) the previous ibos and create 3 new ibos that have a maximum uint32 capacity of count. */
	void Reserve(uint32 count);

	/* Swap the bound and modify buffers to the next one, round robin. */
	void SwapNextBuffer();

	/* Get the mapped ibo data for the bound vbo. See SwapNextBuffer(). */
	MappedIbo& GetBoundMappedIbo() { return ibos[boundId]; }
	/* Get the mapped ibo data for the one that will be swapped in next. See SwapNextBuffer(). */
	MappedIbo& GetModifyMappedIbo() { return ibos[modifyId]; }
	/* Get the amount of uint32 elements that the ibos can hold. */
	uint32 GetCapacity() const { return capacity; }

	IndexBufferObject* GetBoundIbo() const { return ibos[boundId].ibo; }

private:

	/* Delete the pre-existing ibos if they are non-null. */
	void DeleteIbos();

private:

	MappedIbo ibos[3];
	uint32 capacity;
	int boundId;
	int modifyId;

};