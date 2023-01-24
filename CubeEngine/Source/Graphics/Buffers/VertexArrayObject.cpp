#include <glad/glad.h>
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include <vector>

VertexArrayObject::VertexArrayObject()
{
	glGenVertexArrays(1, &id);
}

VertexArrayObject::~VertexArrayObject()
{
	glDeleteVertexArrays(1, &id);
}

void VertexArrayObject::LinkVertexBufferObject(VertexBufferObject* vbo, const VertexBufferLayout& layout)
{
	Bind();
	vbo->Bind();

	darray<VertexBufferLayout::VertexBufferElement> elements = layout.GetElements();
	const uint32 elementCount = elements.Size();
	uint32 offset = 0;

	for (int i = 0; i < elementCount; i++) {
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

	//glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	
	vbo->Unbind();
}

void VertexArrayObject::Bind()
{
	glBindVertexArray(id);
}

void VertexArrayObject::Unbind()
{
	glBindVertexArray(0);
}
