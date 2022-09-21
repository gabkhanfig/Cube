#pragma once

#include <CubeCore.h>
#include <Game/World/WorldPosition.h>

class Chunk;
class Biome;
class Block;

class Dimension 
{
public:

	std::unordered_map<ChunkPosition, Chunk*> loadedChunks;

	Biome* biome;

	int dimId;

	Dimension();
	
	void InitChunks();

	void Tick(float deltaTime);

	Chunk* GetChunk(const ChunkPosition& position);

	Block* GetBlockForWorldPosition(const WorldPosition& worldPos);

	void RemoveDistantChunks();

	void LoadChunksAroundPlayer();

	void RemoveAndLoadChunksAroundPlayer();

	std::vector<Chunk*> GetAdjacentChunks(const ChunkPosition& chunkPos);

	void GenerateMap();
};