#pragma once

#include "../../EngineCore.h"

class IndexBufferObject
{
private:

	uint32 id;

public:

	IndexBufferObject(const uint32* indices, uint32 num);

	~IndexBufferObject();

	void Bind();

	void Unbind();


};

