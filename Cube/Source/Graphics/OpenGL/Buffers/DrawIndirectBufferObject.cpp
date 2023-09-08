#include "DrawIndirectBufferObject.h"
#include <glad/glad.h>
#include "../../../Engine/Engine.h"
#include "../../../Engine/OpenGL/OpenGLInstance.h"

uint32 DrawIndirectBufferObject::boundId = 0;

DrawIndirectBufferObject::DrawIndirectBufferObject()
{
	assertOnRenderThread();
	glCreateBuffers(1, &id);
}

DrawIndirectBufferObject::~DrawIndirectBufferObject()
{
	assertOnRenderThread();
	if (IsBound()) {
		boundId = 0;
	}
	glDeleteBuffers(1, &id);
}

void DrawIndirectBufferObject::BufferData(DrawElementsIndirectCommand* commands, uint32 commandsNum)
{
	assertOnRenderThread();
	glNamedBufferData(id, commandsNum * sizeof(DrawElementsIndirectCommand), commands, GL_STATIC_DRAW);
}

DrawElementsIndirectCommand* DrawIndirectBufferObject::CreatePersistentMappedStorage(uint32 elementCapacity)
{
	assertOnRenderThread();
	const GLBufferStorageBitmask mapFlags = GLBufferStorageBitmask::Write | GLBufferStorageBitmask::Persistent | GLBufferStorageBitmask::Coherent;
	const uint32 bufferCapacity = elementCapacity * sizeof(DrawElementsIndirectCommand);

	glNamedBufferStorage(id, bufferCapacity, nullptr, mapFlags.bitmask);
	DrawElementsIndirectCommand* mappedBufferRange = (DrawElementsIndirectCommand*)glMapNamedBufferRange(id, 0, bufferCapacity, mapFlags.bitmask);
	gk_assertNotNull(mappedBufferRange);
	return mappedBufferRange;
}

void DrawIndirectBufferObject::Bind()
{
	if (IsBound()) return;
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, id);
	boundId = id;
}

void DrawIndirectBufferObject::Unbind()
{
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
	boundId = 0;
}

bool DrawIndirectBufferObject::IsBound() const
{
	return id == boundId;
}
