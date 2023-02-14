#pragma once

#include "BlockGeometry.h"

struct BlockMesh
{
	darray<BlockQuad> quads;

	BlockMesh() {
		quads.Reserve(6); // Reserve for a full cube.
	}

	uint32 QuadCount() const { return quads.Size(); }

	void AddQuad(const BlockQuad& quad) {
		quads.Add(quad);
	}
};