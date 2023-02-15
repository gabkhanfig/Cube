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

	void Shift(glm::vec3 offset) {
		for (ArrSizeT i = 0; i < quads.Size(); i++) {
			quads[i].Shift(offset);
		}
	}
};