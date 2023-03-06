#pragma once

#include "../../../Engine/EngineCore.h"

class VertexBufferObject
{
private:

	static uint32 boundId;

	uint32 id;

public:

	VertexBufferObject(const void* data, uint32 size);

	~VertexBufferObject();

	template<typename T>
	static VertexBufferObject* Create(const T* data, uint32 num) {
		return new VertexBufferObject(data, sizeof(T) * num);
	}

	void Bind();

	void Unbind();

	bool IsBound() const;

	uint32 GetId() const { return id; }
	
};


