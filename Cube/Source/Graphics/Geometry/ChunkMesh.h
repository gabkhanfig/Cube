#pragma once

#include "BlockGeometry.h"

class VertexBufferObject;
class IndexBufferObject;

class ChunkMesh
{
private:

	darray<BlockQuad> quads;

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

	darray<BlockQuad>& GetQuads() { return quads; }

	const darray<BlockQuad>& GetQuads() const { return quads; }

	darray<uint32> GetIndices() const;

	uint32 GetIndexCount() const { return quads.Size() * 6; }

	uint32 GetQuadCount() const { return quads.Size(); }

	/* Pointer needs to be manually freed/deleted. */
	static uint32* CreateQuadIndices(const uint32 quadCount);

};

struct ChunkRenderMeshData 
{
	ChunkRenderMeshData()
		: positionOffset(0, 0, 0)
	{
		mesh = new ChunkMesh();
	}

	~ChunkRenderMeshData() {
		delete mesh;
	}

	ChunkMesh* mesh;
	glm::vec3 positionOffset;
};