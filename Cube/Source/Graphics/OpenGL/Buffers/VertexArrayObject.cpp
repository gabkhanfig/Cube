#include <glad/glad.h>
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "IndexBufferObject.h"
#include "../../../Engine/OpenGL/OpenGLInstance.h"
#include "../../../Engine/Engine.h"

uint32 VertexArrayObject::boundId = 0;

VertexArrayObject::VertexArrayObject()
{
	assertOnRenderThread();
	glCreateVertexArrays(1, &id);
}

VertexArrayObject::~VertexArrayObject()
{
	assertOnRenderThread();
	if (IsBound()) {
		boundId = 0;
	}
	glDeleteVertexArrays(1, &id);
}

void VertexArrayObject::LinkVertexBufferObjectLayout(VertexBufferObject* vbo, const VertexBufferLayout& layout)
{
	Bind();
	vbo->Bind();

	darray<VertexBufferLayout::VertexBufferElement> elements = layout.GetElements();
	const uint32 elementCount = elements.Size();
	uint64 offset = 0;

	for (uint32 i = 0; i < elementCount; i++) {
		const VertexBufferLayout::VertexBufferElement& element = elements[i];
		glEnableVertexAttribArray(i);
		 
		switch (element.type) {
		case GL_FLOAT:
			glVertexAttribPointer(i, element.count, GL_FLOAT, element.normalized, layout.GetStride(), (const void*)offset);
			break;

		case GL_UNSIGNED_INT:
			glVertexAttribIPointer(i, element.count, GL_UNSIGNED_INT, layout.GetStride(), (const void*)offset);
			break;
		}

		offset += element.count * element.size;
	}
	
	vbo->Unbind();
}

void VertexArrayObject::SetFormatLayout(const VertexBufferLayout& layout)
{
	assertOnRenderThread();

	darray<VertexBufferLayout::VertexBufferElement> elements = layout.GetElements();
	const uint32 elementCount = elements.Size();
	uint32 offset = 0;
	
	//Bind();
	//
	//for (uint32 i = 0; i < elementCount; i++) {
	//	const VertexBufferLayout::VertexBufferElement& element = elements[i];
	//	glEnableVertexAttribArray(i);

	//	switch (element.type) {
	//	case GL_FLOAT:
	//		glVertexAttribFormat(i, element.count, GL_FLOAT, element.normalized, offset);
	//		break;

	//	case GL_UNSIGNED_INT:
	//		glVertexAttribIFormat(i, element.count, GL_UNSIGNED_INT, offset);
	//		break;
	//	}

	//	glVertexAttribBinding(i, 0);
	//	offset += element.count * element.size;
	//}

	
	for (uint32 i = 0; i < elementCount; i++) {
		const VertexBufferLayout::VertexBufferElement& element = elements[i];
		glEnableVertexArrayAttrib(id, i);
		glVertexArrayAttribBinding(id, i, 0);
	
		switch (element.type) {
		case GL_FLOAT:
			glVertexArrayAttribFormat(id, i, element.count, GL_FLOAT, element.normalized, offset);
			break;
	
		case GL_UNSIGNED_INT:
			glVertexArrayAttribIFormat(id, i, element.count, GL_UNSIGNED_INT, offset);
			break;
		}
		
		offset += element.count * element.size;
	}
}

void VertexArrayObject::BindVertexBufferObject(VertexBufferObject* vbo, uint32 bytesPerElement)
{
	assertOnRenderThread();
	glVertexArrayVertexBuffer(id, 0, vbo->GetId(), 0, bytesPerElement);
}

void VertexArrayObject::BindIndexBufferObject(IndexBufferObject* ibo)
{
	assertOnRenderThread();
	glVertexArrayElementBuffer(id, ibo->GetId());
}

void VertexArrayObject::Bind()
{
	assertOnRenderThread();
	if (IsBound()) return;
	glBindVertexArray(id);
	boundId = id;
}

void VertexArrayObject::Unbind()
{
	assertOnRenderThread();
	glBindVertexArray(0);
	boundId = 0;
}

bool VertexArrayObject::IsBound() const
{
	return id == boundId;
}
