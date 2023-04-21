#pragma once

#include "../../../Engine/EngineCore.h"

class VertexBufferObject
{
private:

	static uint32 boundId;

	uint32 id;

public:


	/* Create a completely empty VBO. */
	VertexBufferObject();

	VertexBufferObject(const void* data, uint32 size);


	~VertexBufferObject();

	template<typename T>
	static VertexBufferObject* Create(const T* data, uint32 num) {
		return new VertexBufferObject(data, sizeof(T) * num);
	}

	/* Create a VBO that has a persistently mapped buffer. https://www.khronos.org/opengl/wiki/Buffer_Object#Persistent_mapping
	@param capacity: Number of bytes that are required.
	@param mappedBufferOut: A double pointer that will copy the mapped buffer pointer into. Cannot be nullptr. */
	static VertexBufferObject* CreatePersistentMapped(uint32 capacity, void** mappedBufferOut);

	void Bind();

	void Unbind();

	bool IsBound() const;

	uint32 GetId() const { return id; }

	static uint32 GetBoundId() { return boundId; }

	/* Binds and then returns a write only pointer to the map buffer.
	Call UnmapBuffer(); at some point */
	void* GetMapBuffer();

	static void UnmapBuffer();
	
};