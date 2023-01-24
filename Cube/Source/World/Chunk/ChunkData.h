#pragma once

#include <EngineCore.h>
#include "../WorldTransform.h"

/* 16 x 16 x 16 */
#define CHUNK_SIZE 4096

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