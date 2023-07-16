#pragma once

#include "../WorldTransform.h"

class World;
class Chunk;
class Block;

struct MappedAdjacentChunks {

	static const MappedAdjacentChunks Create(const World* world, const ChunkPosition centerChunkPos);

	const Chunk* GetChunk(const ChunkPosition position) const;

	const Block* GetBlock(const WorldPosition position) const;

private:

	const Chunk* _chunks[27];
	// Bottom left
	ChunkPosition _startPos;

};