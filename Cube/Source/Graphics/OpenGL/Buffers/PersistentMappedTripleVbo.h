#pragma once

#include "VertexBufferObject.h"

template<typename T>
class PersistentMappedTripleVbo
{
public:

	struct MappedVbo {
		VertexBufferObject* vbo;
		T* data;
		uint32 size;

		MappedVbo()
			: vbo(nullptr), data(nullptr), size(0)
		{}
	};

	PersistentMappedTripleVbo()
		: capacity(0), boundId(-1), modifyId(-1)
	{}

	~PersistentMappedTripleVbo()
	{
		for (int i = 0; i < 3; i++) {
			VertexBufferObject* vbo = vbos[i].vbo;
			if (vbo) {
				delete vbo;
			}
		}
	}

	/* Amount of T elements to reserve. */
	void Reserve(uint32 count) {
		capacity = count * sizeof(T);
		boundId = 0;
		modifyId = 2;

		for (int i = 0; i < 3; i++) {
			MappedVbo mapped;
			mapped.vbo = VertexBufferObject::CreatePersistentMappedVbo(capacity, (void**) & mapped.data);
			vbos[i] = mapped;
		}
	}

	void SwapNextBuffer() {
		boundId = (boundId + 1) % 3;
		modifyId = (modifyId + 1) % 3;
	}

	/* Get the mapped vbo data for the bound vbo. See SwapNextBuffer(). */
	MappedVbo& GetBoundMappedVbo() { return vbos[boundId]; }
	/* Get the mapped vbo data for the one that will be swapped in next. See SwapNextBuffer(). */
	MappedVbo& GetModifyMappedVbo() { return vbos[modifyId]; }
	/* Get the amount of T elements that the vbo(s) can hold. */
	uint32 GetCapacity() const { return capacity; }

private:

	MappedVbo vbos[3];
	uint32 capacity;
	int boundId;
	int modifyId;

};