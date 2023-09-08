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
	assertOnRenderThread();
	if (IsBound()) return;
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, id);
	boundId = id;
}

void DrawIndirectBufferObject::Unbind()
{
	assertOnRenderThread();
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
	boundId = 0;
}

bool DrawIndirectBufferObject::IsBound() const
{
	return id == boundId;
}

DrawElementsIndirectCommand* DrawIndirectBufferObject::GetMapBuffer(GLMappedBufferAccess access)
{
	assertOnRenderThread();
	DrawElementsIndirectCommand* mappedBuffer = (DrawElementsIndirectCommand*)glMapNamedBuffer(id, static_cast<int>(access));
	gk_assertNotNull(mappedBuffer);
	return mappedBuffer;
}

void DrawIndirectBufferObject::UnmapBuffer()
{
	assertOnRenderThread();
	GLboolean result = glUnmapNamedBuffer(id);
	gk_assertm(result != GL_FALSE, "Error with unmapping vbo buffer");
}

void DrawIndirectBufferObject::SetBufferStorage(const uint64 elementCapacity, const GLBufferStorageBitmask storageFlags)
{
	assertOnRenderThread();
	glNamedBufferStorage(id, elementCapacity * sizeof(DrawElementsIndirectCommand), nullptr, storageFlags.bitmask);
}
