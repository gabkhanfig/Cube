#pragma once

#include "../../../Engine/EngineCore.h"
#include "VertexBufferLayout.h"

class VertexBufferObject;
class IndexBufferObject;

class VertexArrayObject
{
private:

	static uint32 boundId;
	
	uint32 id;

public:

	VertexArrayObject();

	~VertexArrayObject();

	void LinkVertexBufferObjectLayout(VertexBufferObject* vbo, const VertexBufferLayout& layout);

	void SetFormatLayout(const VertexBufferLayout& layout);

	/* Binds a vbo with the current format layout. See SetFormatLayout() */
	void BindVertexBufferObject(VertexBufferObject* vbo, uint32 bytesPerElement);
	/**/
	void BindIndexBufferObject(IndexBufferObject* ibo);

	/* Binds this vertex array object id. If it's already bound it doesn't bother executing the opengl call. */
	void Bind();

	void Unbind();

	bool IsBound() const;

	forceinline uint32 GetId() const { return id; }
};

