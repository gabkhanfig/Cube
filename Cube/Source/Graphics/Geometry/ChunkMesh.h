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

	void Empty();

	void AddQuad(const BlockQuad& quad);

	darray<BlockQuad>& GetQuads() { return quads; }

	/* Pointer needs to be manually freed/deleted. */
	static uint32* CreateQuadIndices(const uint32 quadCount);

};