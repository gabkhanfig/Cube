#pragma once

#include "ChunkSSBO.h"
#include "../../Graphics/Geometry/ChunkMesh.h"

class VertexBufferObject;
class IndexBufferObject;
class ShaderBufferObject;
class VertexArrayObject;
class Shader;
class Chunk;
class ChunkRenderer;

class ChunkRenderComponent
{
private:

	ChunkMesh mesh;

	VertexBufferObject* vbo;

	IndexBufferObject* ibo;

	Chunk* owner;

	bool meshWasRecreated;

public:

	ChunkRenderComponent(Chunk* _owner);

	~ChunkRenderComponent();

	/* Recreates the mesh on the CPU side. Logic must multithread safe, performing no write operations to the chunk, blocks, or OpenGL. */
	void RecreateMesh();

	void MeshToOpenGLObjects();

	void CopyToSameVBO();

	void Draw(ChunkRenderer* renderer);



};