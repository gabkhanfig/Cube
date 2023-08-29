#pragma once

#include "../../../Engine/EngineCore.h"
#include "../OpenGLEnums.h"
#include "../OpenGLStructures.h"

class VertexBufferObject
{
public:

	template<typename T>
	struct MappedRange {
		T* data;
		VertexBufferObject* vbo;
		int64 elementOffset;
		int64 elementCapacity;
		GLBufferMapBitmask access;

		MappedRange() 
			: data(nullptr), vbo(nullptr), elementOffset(0), elementCapacity(0), access(0)
		{}
	};

	/* Create a completely empty VBO. */
	VertexBufferObject();

	~VertexBufferObject();

	template<typename T>
	void BufferData(const T* data, const uint32 elementCount) {
		BufferDataImpl(data, elementCount * sizeof(T));
	}

	template<typename T>
	[[nodiscard]] T* CreatePersistentMappedStorage(const uint32 elementCapacity) {
		return (T*)CreatePersistentMappedStorageImpl(elementCapacity * sizeof(T));
	}

	void Bind();

	void Unbind();

	[[nodiscard]] bool IsBound() const;

	[[nodiscard]] uint32 GetId() const { return id; }

	[[nodiscard]] static uint32 GetBoundId() { return boundId; }

	/* Binds and then returns a write only pointer to the map buffer.
	Call UnmapBuffer(); at some point */
	template<typename T>
	[[nodiscard]] T* GetMapBuffer(GLMappedBufferAccess access) {
		return (T*)GetMapBufferImpl(access);
	}

	void UnmapBuffer();

	template<typename T>
	void SetBufferStorage(const uint32 elementCapacity, const GLBufferStorageBitmask storageFlags) {
		SetBufferStorageImpl(elementCapacity * sizeof(T), storageFlags);
	}

	template<typename T>
	[[nodiscard]] MappedRange<T> MapRange(const int64 elementOffset, const int64 elementCapacity, const GLBufferMapBitmask access) {
		MappedRange<T> range;
		range.data = (T*)MapBufferRangeImpl(elementOffset * sizeof(T), elementCapacity * sizeof(T), access);
		range.vbo = this;
		range.elementOffset = elementOffset;
		range.elementCapacity = elementCapacity;
		range.access = access;
		return range;
	}

private:

	void BufferDataImpl(const void* data, uint32 totalBytes);

	void* CreatePersistentMappedStorageImpl(const uint32 totalByteCapacity);

	void* GetMapBufferImpl(GLMappedBufferAccess access);

	void SetBufferStorageImpl(const uint32 totalByteCapacity, const GLBufferStorageBitmask storageFlags);

	void* MapBufferRangeImpl(const int64 byteOffset, const int64 totalByteCapacity, const GLBufferMapBitmask access);

private:

	static uint32 boundId;

	uint32 id;
	
};


