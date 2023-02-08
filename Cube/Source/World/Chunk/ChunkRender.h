#pragma once

#include "ChunkSSBO.h"

class VertexBufferObject;
class IndexBufferObject;
class ShaderBufferObject;



/*

*/

/* OpenGL buffer objects for chunks. */
struct ChunkRenderBufferObjects
{
	/**/
	VertexBufferObject* vbo;
	
	/**/
	IndexBufferObject* ibo;

	/**/
	ShaderBufferObject* ssbo;

	ChunkRenderBufferObjects() : vbo(nullptr), ibo(nullptr), ssbo(nullptr) {}

	~ChunkRenderBufferObjects() {
		delete vbo;
		delete ibo;
		delete ssbo;
	}

};


class ChunkRenderComponent
{
private:

	ChunkRenderBufferObjects buffers;

public:
};