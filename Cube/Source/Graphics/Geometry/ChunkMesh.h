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

	VertexBufferObject* MakeVertexBufferObject() const;

	IndexBufferObject* MakeIndexBufferObject() const;

	/* Assumes there's enough space in the buffer. */
	void CopyQuadsToBuffer(BlockQuad* buffer) const;

	/* Assumes there's enough space in the buffer. */
	void CopyIndicesToBuffer(uint32* buffer) const;

	void Empty();

	void AddQuad(const BlockQuad& quad);

	darray<BlockQuad>& GetQuads() { return quads; }

	uint32 GetIndexCount() const { return quads.Size() * 6; }

	uint32 GetQuadCount() const { return quads.Size(); }

	/* Pointer needs to be manually freed/deleted. */
	static uint32* CreateQuadIndices(const uint32 quadCount);

};