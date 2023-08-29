#pragma once

#include "../../../Engine/EngineCore.h"
#include "../OpenGLEnums.h"

class IndexBufferObject
{
public:

	/* Creates an empty IBO. */
	IndexBufferObject();

	~IndexBufferObject();

	void BufferData(const uint32* indices, uint32 num);

	[[nodiscard]] uint32* CreatePersistentMappedStorage(uint32 elementCapacity);

	void Bind();

	void Unbind();

	[[nodiscard]] bool IsBound() const;

	[[nodiscard]] uint32 GetIndexCount() const { return indexCount; }

	void SetIndexCount(uint32 newIndexCount) { indexCount = newIndexCount; }

	/* Binds and then returns a write only pointer to the map buffer.
	Call UnmapBuffer(); at some point */
	[[nodiscard]] uint32* GetMapBuffer(GLMappedBufferAccess access);

	void UnmapBuffer();

	[[nodiscard]] forceinline uint32 GetId() const { return id; }

private:

	static uint32 boundId;

	uint32 id;

	uint32 indexCount;

};

