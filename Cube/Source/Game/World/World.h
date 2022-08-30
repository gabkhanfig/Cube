#pragma once

#include <CubeCore.h>
#include <Game/World/WorldPosition.h>

class Dimension;
class Chunk;
class Block;

class World 
{
public:

	static void Init();

	static std::vector<Dimension*> loadedDimensions;

	static void Tick(float deltaTime);

	static Block* GetBlockAtPosition(const WorldPosition& position, uint32 dimensionIndex = 0);
};