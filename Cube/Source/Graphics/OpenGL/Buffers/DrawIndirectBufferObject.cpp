#include "DrawIndirectBufferObject.h"
#include <glad/glad.h>

uint32 DrawIndirectBufferObject::boundId = 0;

DrawIndirectBufferObject* DrawIndirectBufferObject::Create(DrawElementsIndirectCommand* commands, uint32 numOfCommands)
{
	DrawIndirectBufferObject* dbo = new DrawIndirectBufferObject();
	glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(DrawElementsIndirectCommand) * numOfCommands, commands, GL_STATIC_DRAW);
	return dbo;
}

DrawIndirectBufferObject* DrawIndirectBufferObject::CreatePersistentMapped(uint32 commandCapacity, DrawElementsIndirectCommand** mappedBufferOut)
{
	gk_assertm(mappedBufferOut, "mappedBufferOut must be a non-null pointer to copy the mapped buffer to"); 
	DrawIndirectBufferObject* dbo = new DrawIndirectBufferObject();
	GLbitfield mapFlags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
	glBufferStorage(GL_DRAW_INDIRECT_BUFFER, sizeof(DrawElementsIndirectCommand) * commandCapacity, 0, mapFlags);
	DrawElementsIndirectCommand* bufferRange = (DrawElementsIndirectCommand*)glMapBufferRange(GL_DRAW_INDIRECT_BUFFER, 0, sizeof(DrawElementsIndirectCommand) * commandCapacity, mapFlags);
	gk_assertm(bufferRange, "glMapBufferRange returned nullptr");
	*mappedBufferOut = bufferRange;
	return dbo;
}

DrawIndirectBufferObject::~DrawIndirectBufferObject()
{
	if (IsBound()) {
		boundId = 0;
	}
	glDeleteBuffers(1, &id);
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

DrawIndirectBufferObject::DrawIndirectBufferObject()
{
	glGenBuffers(1, &id);
	Bind();
}
