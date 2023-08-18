#pragma once

#include "../../../Engine/EngineCore.h"
#include "../OpenGLEnums.h"

class IndexBufferObject
{


public:

	/* Creates an empty IBO. */
	IndexBufferObject();

	~IndexBufferObject();

	//IndexBufferObject(const uint32* indices, uint32 num);

	void BufferData(const uint32* indices, uint32 num);

	uint32* CreatePersistentMappedStorage(uint32 elementCapacity);


	/* Due to index buffer objects always using uint32s, the capacity is not the bytes, rather the amount of ints. */
	//static IndexBufferObject* CreatePersistentMapped(uint32 capacity, void** mappedBufferOut);

	void Bind();

	void Unbind();

	bool IsBound() const;

	uint32 GetIndexCount() const { return indexCount; }

	void SetIndexCount(uint32 newIndexCount) { indexCount = newIndexCount; }

	/* Binds and then returns a write only pointer to the map buffer.
	Call UnmapBuffer(); at some point */
	uint32* GetMapBuffer(GLMappedBufferAccess access);

	void UnmapBuffer();

	forceinline uint32 GetId() const { return id; }

private:

	static uint32 boundId;

	uint32 id;

	uint32 indexCount;



};

