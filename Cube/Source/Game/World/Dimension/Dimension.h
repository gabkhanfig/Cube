#pragma once

#include <CubeCore.h>
#include <Game/World/WorldPosition.h>

class Chunk;

class Dimension 
{
public:

	std::unordered_map<ChunkPosition, Chunk*> loadedChunks;

	Dimension();

	void InitChunks();

	void Tick(float deltaTime);

	Chunk* GetChunk(const ChunkPosition& position);
};