#include "Renderer.h"

#include <glad/glad.h>
#include "../Buffers/VertexBufferObject.h"
#include "../Buffers/IndexBufferObject.h"

void Renderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::DrawVboTriangles(VertexBufferObject* vbo, IndexBufferObject* ibo)
{
	ibo->Bind();
	vbo->Bind();
	glDrawElements(GL_TRIANGLES, ibo->GetIndexCount(), GL_UNSIGNED_INT, 0);
}
