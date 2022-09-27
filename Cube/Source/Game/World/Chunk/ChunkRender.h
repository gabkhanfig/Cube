#pragma once

#include <CubeCore.h>
#include <Engine/Render/Geometry/Quad.h>

class VertexBuffer;
class IndexBuffer;
class VertexArray;
class Chunk;
class Block;

/* Buffers objects for chunk rendering. */
struct ChunkBuffers 
{
	VertexBuffer* vbo;

	IndexBuffer* ibo;

	VertexArray* vao;

	uint32 quadsToDraw;

	ChunkBuffers() : 
		vbo(nullptr), ibo(nullptr), vao(nullptr), quadsToDraw(0U)
	{}

	~ChunkBuffers() {
		if (vbo) delete vbo;
		if (ibo) delete ibo;
		if (vao) delete vao;
	}

	void ClearBuffers() {
		if (vbo) delete vbo;
		if (ibo) delete ibo;
		if (vao) delete vao;
	}

	void Bind();

	void MakeNewBlockBuffers(const std::vector<BlockQuad>& quads);
};

/* Object that handles individual chunk rendering. */
struct ChunkRender 
{
	ChunkBuffers graphicsBuffers;

	Chunk* owner;

	uint8 canRender : 1;

	ChunkRender(Chunk* _owner);

	void Draw();

	void ConstructBlockMesh();

	void AsyncConstructBlockMesh();
};