#pragma once

#include "../../../Engine/EngineCore.h"
#include "../OpenGLStructures.h"

class DrawIndirectBufferObject
{
public:

	static DrawIndirectBufferObject* Create(DrawElementsIndirectCommand* commands, uint32 numOfCommands);

	static DrawIndirectBufferObject* CreatePersistentMapped(uint32 commandCapacity, DrawElementsIndirectCommand** mappedBufferOut);

	~DrawIndirectBufferObject();

	void Bind();

	void Unbind();

	bool IsBound() const;

	uint32 GetId() const { return id; }

	static uint32 GetBoundId() { return boundId; }

private:

	DrawIndirectBufferObject();

private:

	static uint32 boundId;

	uint32 id;
};