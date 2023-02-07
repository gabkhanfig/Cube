#pragma once

#include "../../EngineCore.h"

class IndexBufferObject
{
private:

	static uint32 boundId;

	uint32 id;

	uint32 indexCount;

public:

	IndexBufferObject(const uint32* indices, uint32 num);

	~IndexBufferObject();

	void Bind();

	void Unbind();

	bool IsBound() const;

	uint32 GetIndexCount() const { return indexCount; }


};

