#pragma once

#include "VertexBufferObject.h"
#include "../../../Engine/OpenGL/OpenGLInstance.h"
#include "../../../Engine/Engine.h"

template<typename T>
class LargeRangedVbo
{
public:

	LargeRangedVbo() 
		: bufferTotalElementCapacity(0)
	{}


private:

	VertexBufferObject vbo;
	uint64 bufferTotalElementCapacity;
	darray<VertexBufferObject::MappedRange<T>> mappedRanges;
};