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
	void RecreateMesh(ChunkMesh* mesh);

	/* See RecreateMesh() */
	static void MultithreadRecreateMeshes(const ChunkRenderer* chunkRenderer, const darray<ChunkRenderComponent*>& components);

	ChunkMesh* GetMesh() const;

	Chunk* GetChunk() const { return chunk; }

private:

private:

	/* The chunk that owns this render component, and who's mesh will be created and drawn from. */
	Chunk* chunk;

};