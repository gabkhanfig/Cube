#pragma once

#include "ChunkData.h"
#include "../WorldTransform.h"

class IBlock;

struct ChunkBlock
{
	IBlock* block;
	BlockLight light;
	BlockFacing facing;

	ChunkBlock();

	~ChunkBlock();
};