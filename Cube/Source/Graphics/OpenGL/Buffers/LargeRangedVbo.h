#pragma once

#include "VertexBufferObject.h"
#include "../../../Engine/OpenGL/OpenGLInstance.h"
#include "../../../Engine/Engine.h"

template<typename T>
class LargeRangedVbo;

template<typename T>
struct VboMappedRangeRef {

	VboMappedRangeRef(VertexBufferObject::MappedRange<T>* mappedRange, LargeRangedVbo<T>* owningBuffer)
		: _mappedRange(mappedRange), _owningBuffer(owningBuffer) {}

	~VboMappedRangeRef() { FreeRange(); }

	void FreeRange();

	void WriteToRange(const T* data, const uint64 count) {
		gk_assertm(count * sizeof(T) <= _mappedRange->elementCapacity, "Cannot copy more data than the range's capacity");
		gk_assertm(_mappedRange->access & GLBufferMapBitmask::Write, "Range must have write access");
		memcpy(_mappedRange->data, data, count * sizeof(T));
	}

	const T* GetRangeData() const {
		gk_assertm(_mappedRange->access & GLBufferMapBitmask::Read, "Range must have read access for const T*");
		return _mappedRange->data;
	}

private:
	VertexBufferObject::MappedRange<T>* _mappedRange;
	LargeRangedVbo<T>* _owningBuffer;
};

template<typename T>
class LargeRangedVbo
{
public:

	LargeRangedVbo();

	~LargeRangedVbo();

	void Reserve(uint64 elementCapacity);

	void FreeRange(VertexBufferObject::MappedRange<T>* range);


private:

	struct CachedSubrange {
		uint64 elementOffset;
		uint64 elementCapacity;
	};

	VertexBufferObject* vbo;
	uint64 bufferTotalElementCapacity;
	darray<CachedSubrange> cachedSubranges;
	darray<VertexBufferObject::MappedRange<T>*> mappedRanges;
};

template<typename T>
inline void VboMappedRangeRef<T>::FreeRange()
{
	if (_owningBuffer) {
		gk_assertNotNull(_mappedRange);
		_owningBuffer->FreeRange(_mappedRange);
		_owningBuffer = nullptr;
	}
	_mappedRange = nullptr;
}

template<typename T>
inline LargeRangedVbo<T>::LargeRangedVbo()
	: bufferTotalElementCapacity(0)
{
	assertOnRenderThread();
	vbo = new VertexBufferObject();
}

template<typename T>
inline LargeRangedVbo<T>::~LargeRangedVbo()
{
	assertOnRenderThread();
	delete vbo;
	for (int i = 0; i < mappedRanges.Size(); i++) {
		delete mappedRanges[i];
	}
}

template<typename T>
inline void LargeRangedVbo<T>::Reserve(uint64 elementCapacity)
{
	assertOnRenderThread();
	gk_assertm(cachedSubranges.Size() == mappedRanges.Size(), "Cached subranges and mapped ranges must have a 1-1 correlation");
	gk_assertm(elementCapacity != 0, "Cannot reserve element capacity of 0");

	elementCapacity = gk::UpperPowerOfTwo(elementCapacity);
	if (elementCapacity <= bufferTotalElementCapacity) return;

	const GLBufferStorageBitmask bufferStorageFlags =
		GLBufferStorageBitmask::Read | GLBufferStorageBitmask::Write | GLBufferStorageBitmask::Persistent | GLBufferStorageBitmask::Coherent;

	if (cachedSubranges.Size() == 0) { // If there are no subranges, just allocate the vbo buffer storage
		if (vbo) {
			delete vbo;
		}
		vbo = new VertexBufferObject();
		vbo->SetBufferStorage<T>(elementCapacity, bufferStorageFlags);
		return;
	}
	// If there are subranges, they will all need to have their data copied over.
	const uint32 subrangesNum = cachedSubranges.Size();

	VertexBufferObject* newVbo = new VertexBufferObject();
	newVbo->SetBufferStorage<T>(elementCapacity, bufferStorageFlags);

	const GLBufferMapBitmask mapRangeBitmask =
		GLBufferMapBitmask::Read | GLBufferMapBitmask::Write | GLBufferMapBitmask::Persistent | GLBufferMapBitmask::Coherent;
	uint64 currentOffset = 0;
	for (uint32 i = 0; i < subrangesNum; i++) {
		CachedSubrange& cachedSubrange = cachedSubranges[i];
		VertexBufferObject::MappedRange<T>* mappedRange = mappedRanges[i];

		VertexBufferObject::MappedRange<T> newRange = newVbo->MapRange(currentOffset, cachedSubrange.elementCapacity, mapRangeBitmask);

		cachedSubrange.elementOffset = currentOffset;

		gk_assertm(mappedRange->access.bitmask & GLBufferMapBitmask::Read, "Must be able to read from the buffer to memcpy for a new allocation");
		memcpy(newRange.data, mappedRange->data, sizeof(T) * cachedSubrange.elementCapacity);

		mappedRange->data = newRange.data;
		mappedRange->access = newRange.access;
		mappedRange->elementOffset = currentOffset;

		// Some sanity checks
		gk_assert(cachedSubrange.elementOffset == cachedSubranges[i].elementOffset);
		gk_assert(cachedSubrange.elementCapacity == cachedSubranges[i].elementCapacity);
		gk_assert(mappedRange->data == mappedRanges[i]->data);
		gk_assert(mappedRange->access == mappedRanges[i]->access);
		gk_assert(mappedRange->elementOffset == mappedRanges[i]->elementOffset);
		gk_assert(mappedRange->elementCapacity == mappedRanges[i]->elementCapacity);

		currentOffset += cachedSubrange.elementCapacity;
	}

	if (vbo) {
		delete vbo;
	}
	vbo = newVbo;
}

template<typename T>
inline void LargeRangedVbo<T>::FreeRange(VertexBufferObject::MappedRange<T>* range)
{
	darray<T>::OptionalIndex optionalIndex = mappedRanges.Find(range);
	gk_assertm(optionalIndex.IsValidIndex(), "Range must exist within the large vbos mapped ranges");

	const uint32 index = optionalIndex.Get();
	cachedSubranges.RemoveAt(index);
	mappedRanges.RemoveAt(index);

	delete range;
}

