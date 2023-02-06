#pragma once

#include "ChunkDataTypes.h"

class Block;

class Chunk
{
private:

	/* Array of blocks with size of either 0 or CHUNK_SIZE */
	ChunkBlock* blocks;

	uint8 blocksEmpty : 1;

public:

	Chunk();
};