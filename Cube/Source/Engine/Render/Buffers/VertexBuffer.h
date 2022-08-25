#pragma once

#include <Engine/Render/Renderer.h>

class VertexBuffer
{
private:

	uint32 rendererId;

public:

	VertexBuffer(const void* data, uint32 size) 
	{
		glGenBuffers(1, &rendererId);
		Bind();
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
	}

	~VertexBuffer() 
	{
		glDeleteBuffers(1, &rendererId);
	}

	inline void Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, rendererId);
	}

	inline void Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
};