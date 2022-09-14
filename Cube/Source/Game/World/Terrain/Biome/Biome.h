#pragma once

#include <CubeCore.h>
#include <Game/World/WorldPosition.h>

class Block;

class Biome 
{
	int biomeId;


public:

	virtual uint32 GetBlockIdForPos(const WorldPosition& worldPos);
	virtual Block* GetBlockForPos(const WorldPosition& worldPos);
};