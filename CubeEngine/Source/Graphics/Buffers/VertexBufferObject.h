#pragma once

#include "../../EngineCore.h"

class VertexBufferObject
{
private:

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
	
};


