#pragma once

#include "ChunkSSBO.h"
#include "../../Graphics/Geometry/ChunkMesh.h"
#include "../../Graphics/OpenGL/OpenGLStructures.h"
#include "../../Graphics/OpenGL/Buffers/PersistentMappedTripleBuffer.h"

class VertexBufferObject;
class IndexBufferObject;
class ShaderBufferObject;
class VertexArrayObject;
class Shader;
class Chunk;
class ChunkRenderer;

class ChunkRenderComponent
{
public:

	ChunkRenderComponent(Chunk* chunkOwner);

	~ChunkRenderComponent();

	/* Recreates the mesh on the CPU side. Logic must multithread safe, performing no write operations to the chunk, blocks, or OpenGL. */
	void RecreateMesh();

	/* See RecreateMesh() */
	static void MultithreadRecreateMeshes(const ChunkRenderer* chunkRenderer, const darray<Chunk*>& chunks);

	Chunk* GetChunk() const { return chunk; }

	ChunkMesh* GetMesh() const { return mesh; };

	PersistentMappedTripleBuffer<VertexBufferObject, BlockQuad>* GetVbos() const { return vbos; }

	PersistentMappedTripleBuffer<IndexBufferObject, uint32>* GetIbos() const { return ibos; }

private:

private:

	/* The chunk that owns this render component, and who's mesh will be created and drawn from. */
	Chunk* chunk;

	/* The mesh quads to be copied to the GPU. */
	ChunkMesh* mesh;

	/* Persistent Mapped Triple Vertex Buffer Objects for rendering. */
	PersistentMappedTripleBuffer<VertexBufferObject, BlockQuad>* vbos;

	/* Persistent Mapped Triple Index Buffer Objects for rendering. */
	PersistentMappedTripleBuffer<IndexBufferObject, uint32>* ibos;

};