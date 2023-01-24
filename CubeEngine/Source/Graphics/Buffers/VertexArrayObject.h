#pragma once

#include "../../EngineCore.h"
#include "VertexBufferLayout.h"

class VertexBufferObject;

class VertexArrayObject
{
private:
	
	uint32 id;

public:

	VertexArrayObject();

	~VertexArrayObject();

	void LinkVertexBufferObject(VertexBufferObject* vbo, const VertexBufferLayout& layout);

	void Bind();

	void Unbind();
};

