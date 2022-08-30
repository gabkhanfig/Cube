#include "World.h"
#include <Game/World/Dimension/Dimension.h>
#include <Game/World/Chunk/Chunk.h>
#include <Game/World/Block/Block.h>

std::vector<Dimension*> World::loadedDimensions;

void World::Init()
{
	Dimension* overworld = new Dimension();
	loadedDimensions.push_back(overworld);
	Print("Loaded overworld");
	overworld->InitChunks();
}

void World::Tick(float deltaTime)
{
	for (Dimension* dimension : loadedDimensions) {
		dimension->Tick(deltaTime);
	}
}

Block* World::GetBlockAtPosition(const WorldPosition& position, uint32 dimensionIndex)
{
	ChunkPosition chunkPos; 
	WorldPosition::ToChunkPosition_Out(chunkPos, position);
	Chunk* chunk = World::loadedDimensions[dimensionIndex]->GetChunk(chunkPos);
	if (chunk == nullptr) {
		return nullptr;
	}
	BlockPosition localPosition = {
		localPosition.x = position.x % CHUNK_WIDTH,
		localPosition.y = position.y % CHUNK_WIDTH,
		localPosition.z = position.z % CHUNK_WIDTH
	};
	return chunk->GetBlockAtLocalPosition(localPosition);
}
