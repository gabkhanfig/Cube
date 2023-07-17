#pragma once

#include "../WorldTransform.h"
#include "BuriedChunkBlocks.h"

class World;
class Chunk;
class Block;

struct MappedAdjacentChunks 
{
	static const MappedAdjacentChunks Create(const World* world, const ChunkPosition centerChunkPos);

	const Chunk* GetChunk(const ChunkPosition position) const;

	const Block* GetBlock(const WorldPosition position) const;

private:

	const Chunk* _chunks[27];
	// Bottom left
	ChunkPosition _startPos;

};

struct MappedAdjacentAndBuriedChunks 
{
	static const MappedAdjacentAndBuriedChunks Create(const World* world, const ChunkPosition centerChunkPos);

	const Chunk* GetChunk(const ChunkPosition position) const;

	const Block* GetBlock(const WorldPosition position) const;

	const bool IsBlockBuried(const WorldPosition position) const;

private:

	BuriedChunkBlocks _buriedBitmasks[27];

	const Chunk* _chunks[27];
	// Bottom left
	ChunkPosition _startPos;


};