#pragma once

#include "ChunkSSBO.h"
#include "../../Graphics/Geometry/ChunkMesh.h"

class VertexBufferObject;
class IndexBufferObject;
class ShaderBufferObject;
class VertexArrayObject;
class Shader;
class Chunk;


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

	ChunkMesh mesh;

	VertexBufferObject* vbo;

	IndexBufferObject* ibo;

	//ChunkRenderBufferObjects buffers;

	Chunk* owner;

	bool meshWasRecreated;

public:

	ChunkRenderComponent(Chunk* _owner);

	~ChunkRenderComponent();

	/* Recreates the mesh on the CPU side. Logic must multithread safe, performing no write operations to the chunk, blocks, or OpenGL. */
	void RecreateMesh();

	void MeshToOpenGLObjects();

	void Draw(Shader* chunkShader, VertexArrayObject* chunkVAO);

};