#pragma once

#include <CubeCore.h>

#define CHUNK_WIDTH 16

struct ChunkPosition
{
	int x;
	int y;
	int z;
};

struct BlockPosition
{
	uint8 x;
	uint8 y;
	uint8 z;
};

struct WorldPosition 
{
	int x;
	int y;
	int z;

	static inline WorldPosition ToWorldPosition(const ChunkPosition& chunk, const BlockPosition block) 
	{
		WorldPosition wp;
		wp.x = chunk.x * 16 + int(block.x);
		wp.y = chunk.y * 16 + int(block.y);
		wp.z = chunk.z * 16 + int(block.z);
		return wp;
	};
};




