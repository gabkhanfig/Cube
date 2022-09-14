#include "Biome.h"
#include <Game/World/Block/Block.h>
#include <Game/World/Block/Ids/BlockIds.h>
#include <Game/World/Block/Factory/BlockFactory.h>

uint32 Biome::GetBlockIdForPos(const WorldPosition& worldPos)
{
	if (worldPos.y > 0) {
		return BlockId_Air;
	}

	return BlockId_StonyGrass;
}

Block* Biome::GetBlockForPos(const WorldPosition& worldPos)
{
	Block* block = BlockFactory::GetBlock(GetBlockIdForPos(worldPos));
	return block;
}


