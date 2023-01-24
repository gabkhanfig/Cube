#pragma once

#include "ChunkSSBO.h"

class VertexBufferObject;
class IndexBufferObject;
class VertexArrayObject;
class ShaderBufferObject;



/*

*/

/* OpenGL buffer objects for chunks. */
struct ChunkBufferObjects
{
	/**/
	VertexBufferObject* vbo;
	
	/**/
	IndexBufferObject* ibo;

	/**/
	VertexArrayObject* vao;

	/**/
	ShaderBufferObject* ssbo;
};


class ChunkBuffers
{
private:

	ChunkBufferObjects buffers;

public:
};