#pragma once

#include "BlockGeometry.h"

//class VertexBufferObject;
//class IndexBufferObject;

class ChunkMesh
{
public:

	ChunkMesh();

	//VertexBufferObject* MakeVertexBufferObject() const;

	//IndexBufferObject* MakeIndexBufferObject() const;

	/* Assumes there's enough space in the buffer. */
	void CopyQuadsToBuffer(BlockQuad* buffer) const;

	/* Assumes there's enough space in the buffer. indexOffset is a number that will be added to every index in the buffer. Useful for multidraw. */
	void CopyIndicesToBuffer(uint32* buffer, uint32 indexOffset) const;

	void Empty();

	void AddQuad(const BlockQuad& quad);

	const BlockQuad* GetQuadsData() const;

	darray<uint32> GetIndices() const;

	uint32 GetIndexCount() const { return quads.Size() * 6; }

	uint32 GetQuadCount() const { return quads.Size(); }

	/* Pointer needs to be manually freed/deleted. */
	static uint32* CreateQuadIndices(const uint32 quadCount);

	void Reserve(uint32 capacity);

private:

	darray<BlockQuad> quads;

};

struct ChunkDrawCommand {
	/**/
	glm::vec3 positionOffset;
	/* Number of indices to draw. */
	uint32  count;
	/* Vertex to start drawing from. */
	int  baseVertex;

	ChunkDrawCommand() : count(0), baseVertex(0) {}

	/*
	void glDrawElementsInstancedBaseVertexBaseInstance(	
		GLenum mode, -> GL_TRIANGLES
 		GLsizei count, -> count
 		GLenum type, -> GL_UNSIGNED_INT
 		void *indices, -> (void*)0
 		GLsizei instancecount, -> 1
 		GLint basevertex, -> baseVertex
 		GLuint baseinstance); -> 0
	*/
};

struct ChunkRenderMeshData 
{
	ChunkRenderMeshData()
		: positionOffset(0, 0, 0), boundId(0), modifyId(1)
	{}

	~ChunkRenderMeshData() {
	}

	ChunkDrawCommand& GetBoundCommand() { return commands[boundId]; }
	ChunkDrawCommand& GetModifyCommand() { return commands[modifyId]; }

	void SwapCommand() {
		boundId = (boundId + 1) % 2;
		modifyId = (modifyId + 1) % 2;
	}

	ChunkMesh mesh;
	glm::vec3 positionOffset;

private:

	ChunkDrawCommand commands[2];
	int boundId;
	int modifyId;

};