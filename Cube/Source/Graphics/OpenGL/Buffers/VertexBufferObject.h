#pragma once

#include "../../../Engine/EngineCore.h"
#include "../OpenGLEnums.h"
#include "../OpenGLStructures.h"

class VertexBufferObject
{
public:

	/* Create a completely empty VBO. */
	VertexBufferObject();

	~VertexBufferObject();

	template<typename T>
	void BufferData(const T* data, const uint32 elementCount) {
		BufferDataImpl(data, elementCount * sizeof(T));
	}

	template<typename T>
	T* CreatePersistentMappedStorage(const uint32 elementCapacity) {
		return (T*)CreatePersistentMappedStorageImpl(elementCapacity * sizeof(T));
	}

	void Bind();

	void Unbind();

	bool IsBound() const;

	uint32 GetId() const { return id; }

	static uint32 GetBoundId() { return boundId; }

	/* Binds and then returns a write only pointer to the map buffer.
	Call UnmapBuffer(); at some point */
	template<typename T>
	T* GetMapBuffer(GLMappedBufferAccess access) {
		return (T*)GetMapBufferImpl(access);
	}

	void UnmapBuffer();

	template<typename T>
	void SetBufferStorage(const uint32 elementCapacity, const GLBufferStorageBitmask storageFlags) {
		SetBufferStorageImpl(elementCapacity * sizeof(T), storageFlags);
	}

private:

	void BufferDataImpl(const void* data, uint32 totalBytes);

	void* CreatePersistentMappedStorageImpl(const uint32 totalByteCapacity);

	void* GetMapBufferImpl(GLMappedBufferAccess access);

	void SetBufferStorageImpl(const uint32 totalByteCapacity, const GLBufferStorageBitmask storageFlags);

private:

	static uint32 boundId;

	uint32 id;
	
};


