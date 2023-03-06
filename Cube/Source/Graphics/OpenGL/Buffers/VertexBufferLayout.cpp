#include "VertexBufferLayout.h"
#include <glad/glad.h>

template<>
void VertexBufferLayout::Push<float>(uint32 num)
{
	constexpr uint32 size = sizeof(float);
	elements.Add({ GL_FLOAT, num, size, GL_FALSE });
	stride += num * size;
}

template<>
void VertexBufferLayout::Push<uint32>(uint32 num)
{
	constexpr uint32 size = sizeof(uint32);
	elements.Add({ GL_UNSIGNED_INT, num, size, GL_FALSE });
	stride += num * size;
}

VertexBufferLayout::VertexBufferLayout()
{
	stride = 0;
}

VertexBufferLayout::~VertexBufferLayout()
{
}
