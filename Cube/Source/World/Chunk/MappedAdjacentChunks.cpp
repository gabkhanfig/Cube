#include "MappedAdjacentChunks.h"
#include "../World.h"
#include "Chunk.h"

const MappedAdjacentChunks MappedAdjacentChunks::Create(const World* world, const ChunkPosition centerChunkPos)
{
	gk_assertNotNull(world);
	MappedAdjacentChunks adjacent;
	const ChunkPosition startPos = ChunkPosition(centerChunkPos.x - 1, centerChunkPos.y - 1, centerChunkPos.z - 1);
	adjacent._startPos = startPos;

	int index = 0;
	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			for (int z = 0; z < 3; z++) {
				adjacent._chunks[index] = world->GetChunk(ChunkPosition(startPos.x + x, startPos.y + y, startPos.z + z));
				index++;
			}
		}
	}

	return adjacent;
}

const Chunk* MappedAdjacentChunks::GetChunk(const ChunkPosition position) const
{
	gk_assertm(position.x >= _startPos.x && position.x < _startPos.x + 3, "Position x value is out of range of +0 -> +2 of starting position x\nposition x: " << position.x << "\nstart position x: " << _startPos.x);
	gk_assertm(position.y >= _startPos.y && position.y < _startPos.y + 3, "Position y value is out of range of +0 -> +2 of starting position x\nposition y: " << position.y << "\nstart position y: " << _startPos.y);
	gk_assertm(position.z >= _startPos.z && position.z < _startPos.z + 3, "Position z value is out of range of +0 -> +2 of starting position x\nposition z: " << position.z << "\nstart position z: " << _startPos.z);
	ChunkPosition offset = ChunkPosition(position.x - _startPos.x, position.y - _startPos.y, position.z - _startPos.z);
	const int index = offset.x + offset.y * 3 + offset.z * 9;
	const Chunk* chunk = _chunks[index];
	gk_assertm(chunk->GetPosition() == position,
		"Chunk position does not match the position of the function argument\nchunk position: " << String::From(chunk->GetPosition()) << "\narg position: " << String::From(position));
	return chunk;
}

const Block* MappedAdjacentChunks::GetBlock(const WorldPosition position) const
{
	return GetChunk(position.ToChunkPosition())->GetBlock(position.ToBlockPosition());
}