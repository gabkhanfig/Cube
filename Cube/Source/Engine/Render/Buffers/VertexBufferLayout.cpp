#include "VertexBufferLayout.h"
#include <Engine/Render/Geometry/Vertex.h>

constexpr uint8 SizeOfGLFloat = 4;
constexpr uint8 SizeOfGLUnsignedInt = 4;

template<>
void VertexBufferLayout::Push<float>(uint32 count)
{
	elements.push_back({ GL_FLOAT, count, SizeOfGLFloat, GL_FALSE });
	stride += count * SizeOfGLFloat;
}

template<>
void VertexBufferLayout::Push<uint32>(uint32 count)
{
	elements.push_back({ GL_UNSIGNED_INT, count, SizeOfGLUnsignedInt, GL_FALSE });
	stride += count * SizeOfGLUnsignedInt;
}

VertexBufferLayout VertexBufferLayout::MakeVertexBufferLayoutForVertex()
{
	VertexBufferLayout layout;
	layout.Push<float>(Vertex::GetPositionQuantity());
	layout.Push<float>(Vertex::GetColorQuantity());
	layout.Push<float>(Vertex::GetTextureUVQuantity());
	layout.Push<float>(Vertex::GetTextureLayerQuantity());
	return layout;
}

VertexBufferLayout VertexBufferLayout::MakeVertexBufferLayoutForBlock()
{
	VertexBufferLayout layout;
	layout.Push<uint32>(1); // pos
	layout.Push<uint32>(1); // col
	layout.Push<uint32>(1); // texID
	return layout;
}
