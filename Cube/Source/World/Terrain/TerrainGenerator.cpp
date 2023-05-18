#include "TerrainGenerator.h"
#include "../Block/Block.h"
#include "../Chunk/Chunk.h"
#include "../World.h"
#include "../Block/BlockTypes/Air/AirBlock.h"

Block* TerrainGenerator::CreateBlockForWorldPosition(WorldPosition pos) const
{
	if (pos.y >= 0) {
		return BlockFactory::GetBlockClass(AirBlock::GetStaticName())->NewBlock();
	}
	else {
		return BlockFactory::GetBlockClass("stoneBlock")->NewBlock();
	}
}
