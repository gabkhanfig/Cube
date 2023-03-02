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

};