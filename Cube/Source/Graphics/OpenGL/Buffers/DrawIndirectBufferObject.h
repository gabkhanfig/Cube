#pragma once

#include "../../../Engine/EngineCore.h"
#include "../OpenGLStructures.h"

class DrawIndirectBufferObject
{
public:

	DrawIndirectBufferObject(); 
	
	~DrawIndirectBufferObject();

	void BufferData(DrawElementsIndirectCommand* commands, uint32 commandsNum);

	[[nodiscard]] DrawElementsIndirectCommand* CreatePersistentMappedStorage(uint32 elementCapacity);

	void Bind();

	void Unbind();

	bool IsBound() const;

	uint32 GetId() const { return id; }

	static uint32 GetBoundId() { return boundId; }

private:

	static uint32 boundId;

	uint32 id;
};