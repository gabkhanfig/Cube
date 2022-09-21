#include "World.h"
#include <Game/World/Dimension/Dimension.h>
#include <Game/World/Chunk/Chunk.h>
#include <Game/World/Block/Block.h>
#include <Game/Player/Player.h>
#include <Engine/Input/UserInput.h>

std::vector<Dimension*> World::loadedDimensions;

void World::Init()
{
	Dimension* overworld = new Dimension();
	loadedDimensions.push_back(overworld);
	Print("Loaded overworld");
	overworld->InitChunks();
	UserInput::MapInput(GLFW_KEY_F, World::TestGenNewChunksAroundPlayer);
}

void World::Tick(float deltaTime)
{
	for (Dimension* dimension : loadedDimensions) {
		dimension->Tick(deltaTime);
	}

	Player::GetPlayer()->Tick(deltaTime);
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
		localPosition.x = std::abs(position.x) % CHUNK_WIDTH,
		localPosition.y = std::abs(position.y) % CHUNK_WIDTH,
		localPosition.z = std::abs(position.z) % CHUNK_WIDTH
	};
	//Block* block = chunk->GetBlockAtLocalPosition(localPosition);
	return chunk->GetBlockAtLocalPosition(localPosition);
}

void World::TestGenNewChunksAroundPlayer(int mods)
{
	loadedDimensions[0]->RemoveAndLoadChunksAroundPlayer();
}
