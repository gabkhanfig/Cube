#pragma once

#include "ChunkData.h"
#include "../WorldTransform.h"

class Block;

struct ChunkBlock
{
	Block* block;
	glm::vec3 light;
	BlockFacing facing;

	ChunkBlock()
		: block(nullptr), light({ 0, 0, 0 }), facing(0)
	{}
};