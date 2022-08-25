#pragma once

#include <Engine/Render/Renderer.h>

struct VertexBufferElement
{
	uint32 type;
	uint32 count;
	uint8 size;
	bool normalized;
};

class VertexBufferLayout
{
private:

	std::vector<VertexBufferElement> elements;
	uint32 stride;

public:

	template<typename T>
	void Push(uint32 count);

	template<>
	void Push<float>(uint32 count);

	template<>
	void Push<uint32>(uint32 count);


	inline std::vector<VertexBufferElement> GetElements() const { return elements; }
	inline uint32 GetStride() const { return stride; }

	static VertexBufferLayout MakeVertexBufferLayoutForVertex();

	static VertexBufferLayout MakeVertexBufferLayoutForBlock();
};