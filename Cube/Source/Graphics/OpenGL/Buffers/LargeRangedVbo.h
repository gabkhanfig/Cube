#pragma once

#include "VertexBufferObject.h"
#include "../../../Engine/OpenGL/OpenGLInstance.h"
#include "../../../Engine/Engine.h"

template<typename T>
class LargeRangedVbo;

template<typename T>
struct VboMappedRangeRef {

	VboMappedRangeRef(VertexBufferObject::MappedRange<T>* mappedRange, LargeRangedVbo<T>* owningBuffer)
		: _mappedRange(mappedRange), _owningBuffer(owningBuffer) 
	{}

	~VboMappedRangeRef() { FreeRange(); }

	void FreeRange();

	void WriteToRange(const T* data, const uint64 count) {
		gk_assertm(count <= _mappedRange->elementCapacity, "Cannot copy more data than the range's capacity");
		gk_assertm(_mappedRange->access.bitmask & GLBufferMapBitmask::Write, "Range must have write access");
		memcpy(_mappedRange->data, data, count * sizeof(T));
	}
	
	const T* GetRangeData() const {
		gk_assertm(_mappedRange->access & GLBufferMapBitmask::Read, "Range must have read access for const T*");
		return _mappedRange->data;
	}

	forceinline uint64 GetCapacity() const { return _mappedRange->elementCapacity; }

	forceinline uint64 GetOffset() const { return _mappedRange->elementOffset; }

	forceinline GLBufferMapBitmask GetRangeAccess() const { return _mappedRange->access; }

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

	VboMappedRangeRef<T>* NewMappedRange(const uint64 elementCapacity);

	VertexBufferObject* GetVbo() { return vbo; }

private:

	struct CachedSubrange {
		uint64 elementOffset;
		uint64 elementCapacity;
	};

	VertexBufferObject* vbo;
	T* persistentBuffer;
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

	//elementCapacity = gk::UpperPowerOfTwo(elementCapacity);
	if (elementCapacity <= bufferTotalElementCapacity) return;

	const GLBufferStorageBitmask bufferStorageFlags =
		GLBufferStorageBitmask::Read | GLBufferStorageBitmask::Write | GLBufferStorageBitmask::Persistent | GLBufferStorageBitmask::Coherent;
	const GLBufferMapBitmask mapRangeBitmask =
		GLBufferMapBitmask::Read | GLBufferMapBitmask::Write | GLBufferMapBitmask::Persistent | GLBufferMapBitmask::Coherent;

	if (cachedSubranges.Size() == 0) { // If there are no subranges, just allocate the vbo buffer storage
		if (vbo) {
			delete vbo;
		}
		vbo = new VertexBufferObject();
		vbo->SetBufferStorage<T>(elementCapacity, bufferStorageFlags);
		VertexBufferObject::MappedRange<T>* range = vbo->MapRange<T>(0, elementCapacity, mapRangeBitmask);
		persistentBuffer = range->data;
		delete range;
		bufferTotalElementCapacity = elementCapacity;
		return;
	}
	// If there are subranges, they will all need to have their data copied over.
	const uint32 subrangesNum = cachedSubranges.Size();

	VertexBufferObject* newVbo = new VertexBufferObject();
	newVbo->SetBufferStorage<T>(elementCapacity, bufferStorageFlags);
	VertexBufferObject::MappedRange<T>* newVboRange = newVbo->MapRange<T>(0, elementCapacity, mapRangeBitmask);
	persistentBuffer = newVboRange->data;
	delete newVboRange;

	
	uint64 currentOffset = 0;
	for (uint32 i = 0; i < subrangesNum; i++) {
		CachedSubrange& cachedSubrange = cachedSubranges[i];
		VertexBufferObject::MappedRange<T>* mappedRange = mappedRanges[i];

		//VertexBufferObject::MappedRange<T>* newRange = newVbo->MapRange<T>(currentOffset, cachedSubrange.elementCapacity, mapRangeBitmask);

		T* newRangeData = persistentBuffer + currentOffset;


		cachedSubrange.elementOffset = currentOffset;

		gk_assertm(mappedRange->access.bitmask & GLBufferMapBitmask::Read, "Must be able to read from the buffer to memcpy for a new allocation");
		memcpy(newRangeData, mappedRange->data, sizeof(T) * cachedSubrange.elementCapacity);

		mappedRange->data = newRangeData;
		mappedRange->access = mapRangeBitmask;
		mappedRange->elementOffset = currentOffset;
		//delete newRange;

		cachedSubrange.elementOffset = currentOffset;

		// Some sanity checks
		gk_assert(cachedSubrange.elementOffset == cachedSubranges[i].elementOffset);
		gk_assert(cachedSubrange.elementCapacity == cachedSubranges[i].elementCapacity);
		gk_assert(mappedRange->data == mappedRanges[i]->data);
		gk_assert(mappedRange->access.bitmask == mappedRanges[i]->access.bitmask);
		gk_assert(mappedRange->elementOffset == mappedRanges[i]->elementOffset);
		gk_assert(mappedRange->elementCapacity == mappedRanges[i]->elementCapacity);

		currentOffset += cachedSubrange.elementCapacity;
	}

	if (vbo) {
		delete vbo;
	}
	vbo = newVbo;
	bufferTotalElementCapacity = elementCapacity;
}

template<typename T>
inline void LargeRangedVbo<T>::FreeRange(VertexBufferObject::MappedRange<T>* range)
{
	const auto optionalIndex = mappedRanges.Find(range);
	gk_assertm(optionalIndex.IsValidIndex(), "Range must exist within the large vbos mapped ranges");

	const uint32 index = optionalIndex.Get();
	cachedSubranges.RemoveAt(index);
	mappedRanges.RemoveAt(index);

	delete range;
}

template<typename T>
inline VboMappedRangeRef<T>* LargeRangedVbo<T>::NewMappedRange(const uint64 elementCapacity)
{
	assertOnRenderThread();
	gk_assertm(cachedSubranges.Size() == mappedRanges.Size(), "Cached subranges and mapped ranges must have a 1-1 correlation");
	gk_assertm(elementCapacity != 0, "Cannot reserve element capacity of 0");

	const GLBufferMapBitmask mapRangeBitmask =
		GLBufferMapBitmask::Read | GLBufferMapBitmask::Write | GLBufferMapBitmask::Persistent | GLBufferMapBitmask::Coherent; //| GLBufferMapBitmask::InvalidateRange;

	if (cachedSubranges.Size() == 0) { // If no elements current, just instantly allocate.
		if (bufferTotalElementCapacity < elementCapacity) {
			const uint64 newCapacity = (bufferTotalElementCapacity + elementCapacity) * 2;
			Reserve(newCapacity);
		}

		//VertexBufferObject::MappedRange<T>* newRangeData = vbo->MapRange<T>(0, elementCapacity, mapRangeBitmask); // maybe elementrangeend + 1
		VertexBufferObject::MappedRange<T>* newRangeData = new VertexBufferObject::MappedRange<T>();
		newRangeData->data = persistentBuffer;
		newRangeData->elementOffset = 0;
		newRangeData->elementCapacity = elementCapacity;
		newRangeData->access = mapRangeBitmask;

		CachedSubrange cachedSubrange;
		cachedSubrange.elementOffset = newRangeData->elementOffset;
		cachedSubrange.elementCapacity = newRangeData->elementCapacity;
		cachedSubranges.Add(cachedSubrange);
		mappedRanges.Add(newRangeData);

		VboMappedRangeRef<T>* rangeRef = new VboMappedRangeRef<T>(newRangeData, this);
		return rangeRef;
	}

	for (uint32 i = 0; i < cachedSubranges.Size(); i++) {
		const bool isLastElement = i == (cachedSubranges.Size() - 1);
		
		const uint64 elementRangeEnd = cachedSubranges[i].elementOffset + cachedSubranges[i].elementCapacity;
		if (isLastElement) {
			const uint64 capacityLeftover = bufferTotalElementCapacity - elementRangeEnd;
			if (capacityLeftover > elementCapacity) { // Can create the range
				//VertexBufferObject::MappedRange<T>* newRangeData = vbo->MapRange<T>(elementRangeEnd + 10, elementCapacity, mapRangeBitmask); // maybe elementrangeend + 1
				VertexBufferObject::MappedRange<T>* newRangeData = new VertexBufferObject::MappedRange<T>();
				newRangeData->data = persistentBuffer + elementRangeEnd;
				newRangeData->elementOffset = elementRangeEnd;
				newRangeData->elementCapacity = elementCapacity;
				newRangeData->access = mapRangeBitmask;

				CachedSubrange cachedSubrange;
				cachedSubrange.elementOffset = newRangeData->elementOffset;
				cachedSubrange.elementCapacity = newRangeData->elementCapacity;
				cachedSubranges.Add(cachedSubrange);
				mappedRanges.Add(newRangeData);

				VboMappedRangeRef<T>* rangeRef = new VboMappedRangeRef<T>(newRangeData, this);
				return rangeRef;
			}
			else { // Cannot create the range, so need to reallocate, and then append to end of both arrays.
				const uint64 newCapacity = (bufferTotalElementCapacity + elementCapacity) * 2;
				Reserve(newCapacity);
				
				const uint32 lastIndex = cachedSubranges.Size() - 1;
				const uint64 lastElementRangeEnd = cachedSubranges[lastIndex].elementOffset + cachedSubranges[lastIndex].elementCapacity;

				gk_assertm((bufferTotalElementCapacity - lastElementRangeEnd) > elementCapacity, 
					"Reallocating new capacity for large ranged vbo should've created a large enough capacity to make the new required range");

				//VertexBufferObject::MappedRange<T>* newRangeData = vbo->MapRange<T>(lastElementRangeEnd + 1, elementCapacity, mapRangeBitmask); // maybe elementrangeend + 1
				VertexBufferObject::MappedRange<T>* newRangeData = new VertexBufferObject::MappedRange<T>();
				newRangeData->data = persistentBuffer + lastElementRangeEnd;
				newRangeData->elementOffset = lastElementRangeEnd;
				newRangeData->elementCapacity = elementCapacity;
				newRangeData->access = mapRangeBitmask;

				CachedSubrange cachedSubrange;
				cachedSubrange.elementOffset = newRangeData->elementOffset;
				cachedSubrange.elementCapacity = newRangeData->elementCapacity;
				cachedSubranges.Add(cachedSubrange);
				mappedRanges.Add(newRangeData);

				VboMappedRangeRef<T>* rangeRef = new VboMappedRangeRef<T>(newRangeData, this);
				return rangeRef;
			}
		}
		else {
			const uint64 capacityBetweenNextElement = cachedSubranges[i + 1].elementOffset - elementRangeEnd;
			if (capacityBetweenNextElement <= elementCapacity) {
				continue;
			}

			//VertexBufferObject::MappedRange<T>* newRangeData = vbo->MapRange<T>(elementRangeEnd + 1, elementCapacity, mapRangeBitmask); // maybe elementrangeend + 1
			VertexBufferObject::MappedRange<T>* newRangeData = new VertexBufferObject::MappedRange<T>();
			newRangeData->data = persistentBuffer + elementRangeEnd;
			newRangeData->elementOffset = elementRangeEnd;
			newRangeData->elementCapacity = elementCapacity;
			newRangeData->access = mapRangeBitmask;

			CachedSubrange cachedSubrange;
			cachedSubrange.elementOffset = newRangeData->elementOffset;
			cachedSubrange.elementCapacity = newRangeData->elementCapacity;
			cachedSubranges.Add(cachedSubrange);
			mappedRanges.Add(newRangeData);

			VboMappedRangeRef<T>* rangeRef = new VboMappedRangeRef<T>(newRangeData, this);
			return rangeRef;
		}
	}
}

