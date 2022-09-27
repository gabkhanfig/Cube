#include "VertexArray.h"
#include <Engine/Render/Buffers/VertexBuffer.h>
#include <Engine/Render/Buffers/VertexBufferLayout.h>

void VertexArray::AddBuffer(VertexBuffer* vb, const VertexBufferLayout& layout)
{
	Bind();
	//vb->Bind();
	const std::vector<VertexBufferElement>& elements = layout.GetElements();
	uint32 offset = 0;
	for (uint32 i = 0; i < elements.size(); i++) {
		const VertexBufferElement& element = elements[i];
		glEnableVertexAttribArray(i);
		if (element.type == GL_FLOAT) {
			glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);
		}
		else if (element.type == GL_UNSIGNED_INT) {
			glVertexAttribIPointer(i, element.count, element.type, layout.GetStride(), (const void*)offset);
		}
		offset += element.count * element.size;
	}
}
