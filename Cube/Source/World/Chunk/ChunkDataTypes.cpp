#include "ChunkDataTypes.h"
#include "../Block/IBlock.h"

ChunkBlock::ChunkBlock()
{
	IBlock* airBlock = BlockFactory::GetAirBlock();
	block = airBlock;
}

ChunkBlock::~ChunkBlock()
{
	if (block) {
		block->Destroy();
	}
}

void ChunkBlock::ReplaceBlock(IBlock* newBlock)
{
	checkm(newBlock, "ChunkBlock block cannot be nullptr");
	if (block) {
		block->Destroy();
	}
	block = newBlock;
}

void ChunkBlock::DestroyBlock()
{
	if (block) {
		block->Destroy();
	}
	block = BlockFactory::GetAirBlock();
}

void ChunkBlock::SetLight(BlockLight newLight)
{
	light = newLight;
}

void ChunkBlock::SetFacing(BlockFacing newFacing)
{
	facing = newFacing;
}
