#pragma once
#include <Engine/Render/Renderer.h>

class IndexBuffer 
{
private:

	uint32 rendererId;
	uint32 count;

public:

	IndexBuffer(const uint32* data, uint32 _count) 
	{
		count = _count;

		glGenBuffers(1, &rendererId);
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _count * sizeof(uint32), data, GL_DYNAMIC_DRAW);
	}

	~IndexBuffer() 
	{
		glDeleteBuffers(1, &rendererId);
	}

	inline void Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererId);
	}

	inline void Unbind() const 
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	inline uint32 GetCount() const { return count; }

	/* Make an index buffer specifically for one or more quads, and bind the buffer. */
	static IndexBuffer* MakeQuadsIndexBuffer(uint32 quadCount) 
	{
		const uint32 indicesSize = quadCount * 6;
		uint32* indices = new uint32[indicesSize];

		uint32 index = 0;
		for (uint32 i = 0; i < quadCount; i++) {
			indices[index++] = ((i * 4));
			indices[index++] = (1 + (i * 4));
			indices[index++] = (2 + (i * 4));
			indices[index++] = (2 + (i * 4));
			indices[index++] = (3 + (i * 4));
			indices[index++] = (0 + (i * 4));
		}
		IndexBuffer* ibo = new IndexBuffer(indices, indicesSize);
		delete[] indices;
		return ibo;
	}

};