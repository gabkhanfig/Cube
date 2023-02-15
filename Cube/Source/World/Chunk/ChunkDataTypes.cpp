#include "ChunkDataTypes.h"
#include "../Block/IBlock.h"

ChunkBlock::ChunkBlock()
{
	IBlock* airBlock = BlockFactory::GetAirBlock();
	block = airBlock;
}

ChunkBlock::~ChunkBlock()
{
	block->Destroy();
}
