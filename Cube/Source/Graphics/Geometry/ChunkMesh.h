#pragma once

#include "BlockMesh.h"

class VertexBufferObject;
class IndexBufferObject;

class ChunkMesh
{
private:

	darray<BlockQuad> quads;

public:

	ChunkMesh();

	void AddBlockMesh(const BlockMesh& mesh);

	VertexBufferObject* MakeVertexBufferObject() const;

	IndexBufferObject* MakeIndexBufferObject() const;

};