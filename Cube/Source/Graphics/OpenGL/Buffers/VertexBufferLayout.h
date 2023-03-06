#pragma once

#include "../../../Engine/EngineCore.h"

struct VertexBufferLayout
{

public:

	struct VertexBufferElement
	{
		uint32 type;
		uint32 count;
		uint32 size;
		bool normalized;
	};

private:

	darray<VertexBufferLayout::VertexBufferElement> elements;

	uint32 stride;

public:

	VertexBufferLayout();

	~VertexBufferLayout();

	template<typename T>
	void Push(uint32 num);

	template<>
	void Push<float>(uint32 num);

	template<>
	void Push<uint32>(uint32 count);

	darray<VertexBufferLayout::VertexBufferElement> GetElements() const { return elements; }

	uint32 GetStride() const { return stride; }

};

