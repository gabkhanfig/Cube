#include "MappedAdjacentChunks.h"
#include "../World.h"
#include "Chunk.h"
#include "ChunkRenderComponent.h"

static int GetMappedAdjacentOffsetIndex(const ChunkPosition position, const ChunkPosition startPos) {
	gk_assertm(position.x >= startPos.x && position.x < startPos.x + 3, "Position x value is out of range of +0 -> +2 of starting position x\nposition x: " << position.x << "\nstart position x: " << startPos.x);
	gk_assertm(position.y >= startPos.y && position.y < startPos.y + 3, "Position y value is out of range of +0 -> +2 of starting position x\nposition y: " << position.y << "\nstart position y: " << startPos.y);
	gk_assertm(position.z >= startPos.z && position.z < startPos.z + 3, "Position z value is out of range of +0 -> +2 of starting position x\nposition z: " << position.z << "\nstart position z: " << startPos.z);
	ChunkPosition offset = ChunkPosition(position.x - startPos.x, position.y - startPos.y, position.z - startPos.z);
	const int index = offset.x * 9 + offset.y * 3 + offset.z;
	return index;
}

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
	const int index = GetMappedAdjacentOffsetIndex(position, _startPos);
	return _chunks[index];
	//if (chunk == nullptr) return nullptr;
	//gk_assertm(chunk->GetPosition() == position,
		//"Chunk position does not match the position of the function argument\nchunk position: " << String::From(chunk->GetPosition()) << "\narg position: " << String::From(position));
}

const Block* MappedAdjacentChunks::GetBlock(const WorldPosition position) const
{
	const Chunk* chunk = GetChunk(position.ToChunkPosition());
	return chunk == nullptr ? nullptr : chunk->GetBlock(position.ToBlockPosition());
}

const MappedAdjacentAndBuriedChunks MappedAdjacentAndBuriedChunks::Create(const World* world, const ChunkPosition centerChunkPos)
{
	gk_assertNotNull(world);
	MappedAdjacentAndBuriedChunks adjacent;
	const ChunkPosition startPos = ChunkPosition(centerChunkPos.x - 1, centerChunkPos.y - 1, centerChunkPos.z - 1);
	adjacent._startPos = startPos;

	int index = 0;
	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			for (int z = 0; z < 3; z++) {
				Chunk* chunk = world->GetChunk(ChunkPosition(startPos.x + x, startPos.y + y, startPos.z + z));
				if (chunk == nullptr) {
					adjacent._chunks[index] = nullptr; 
					adjacent._buriedBitmasks[index] = ChunkBlocksBitmask();
					index++;
					continue;
				}
				adjacent._chunks[index] = world->GetChunk(ChunkPosition(startPos.x + x, startPos.y + y, startPos.z + z));
				adjacent._buriedBitmasks[index] = chunk->GetRenderComponent()->GetBuriedBlocksBitmask();
				index++;
			}
		}
	}

	return adjacent;
}

const Chunk* MappedAdjacentAndBuriedChunks::GetChunk(const ChunkPosition position) const
{
	const int index = GetMappedAdjacentOffsetIndex(position, _startPos);
	const Chunk* chunk = _chunks[index];
	if (chunk == nullptr) return nullptr;
	gk_assertm(chunk->GetPosition() == position,
		"Chunk position does not match the position of the function argument\nchunk position: " << String::From(chunk->GetPosition()) << "\narg position: " << String::From(position));
	return chunk;
}

const Block* MappedAdjacentAndBuriedChunks::GetBlock(const WorldPosition position) const
{
	return GetChunk(position.ToChunkPosition())->GetBlock(position.ToBlockPosition());
}

const bool MappedAdjacentAndBuriedChunks::IsBlockBuried(const WorldPosition position) const
{
	const ChunkPosition chunkPos = position.ToChunkPosition();
	const int chunkIndex = GetMappedAdjacentOffsetIndex(chunkPos, _startPos);
	return _buriedBitmasks[chunkIndex].GetFlag(position.ToBlockPosition());

}
