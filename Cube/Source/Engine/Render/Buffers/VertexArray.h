#pragma once

#include <Engine/Render/Renderer.h>

class VertexBuffer;
class VertexBufferLayout;

class VertexArray
{
private:

	uint32 rendererId;

public:

	VertexArray() 
	{
		glGenVertexArrays(1, &rendererId);
	}

	~VertexArray() 
	{
		glDeleteVertexArrays(1, &rendererId);
	}

	inline void Bind() const
	{
		glBindVertexArray(rendererId);
	}

	void AddBuffer(VertexBuffer* vb, const VertexBufferLayout& layout);

};