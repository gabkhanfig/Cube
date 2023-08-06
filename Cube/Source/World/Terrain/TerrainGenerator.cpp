#include "TerrainGenerator.h"
#include "../Chunk/Chunk.h"
#include "../World.h"
#include "../Block/BlockFactory.h"

Block TerrainGenerator::CreateBlockForWorldPosition(WorldPosition pos) const
{
	return BlockFactory::CreateBlock("stone");
	//if (pos.y >= 0) {
	//	return BlockFactory::GetBlockClass(AirBlock::GetStaticName())->NewBlock();
	//}
	//else {
	//	return BlockFactory::GetBlockClass("stoneBlock")->NewBlock();
	//}
}
