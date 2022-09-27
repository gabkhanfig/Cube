#include "ChunkBlocks.h"
#include <iostream>

#define CHUNK_SIZE 4096

#include <Game/World/Block/Factory/BlockFactory.h>
#include <Game/World/Block/Ids/BlockIds.h>
#include <Game/World/Block/Block.h>

ChunkBlocks::ChunkBlocks()
{
	std::cout << "chunk blocks constructor" << std::endl;
	airBlock = BlockFactory::GetBlock(BlockId_Air);
	blocks = nullptr;
	blocksMode = BlocksMode::Default;
}

ChunkBlocks::~ChunkBlocks()
{
	for (int i = 0; i < CHUNK_SIZE; i++) {
		blocks[i]->Destroy();
	}

	delete[] blocks;
}

Block* ChunkBlocks::GetBlock(int index)
{
	if (blocksMode == BlocksMode::AirFilled) {
		return airBlock;
	}

	if (blocks == nullptr) {
		return airBlock;
	}

	return blocks[index];
}

Block* ChunkBlocks::GetBlock(BlockPosition pos)
{
	const int index = BlockPosition::ToBlockIndex(pos);
	return GetBlock(index);
}

bool ChunkBlocks::CheckIfShouldBeAirFilled()
{
	if (blocks == nullptr) return false;

	int airCount = 0;
	for (int i = 0; i < CHUNK_SIZE; i++) {
		if (GetBlock(i) == airBlock) {
			airCount++;
		}
	}

	if (airCount == (CHUNK_SIZE - 1)) {
		ClearBlocks();
		blocksMode == BlocksMode::AirFilled;
		return true;
	}
	
	return false;
}

void ChunkBlocks::ClearBlocks()
{
	for (int i = 0; i < CHUNK_SIZE; i++) {
		blocks[i]->Destroy();
	}

	delete[] blocks;
	blocks = nullptr;
}

void ChunkBlocks::TestSetAllBlocks()
{
	Block* grass = BlockFactory::GetBlock(BlockId_Stone);
	blocks = new Block * [CHUNK_SIZE];
	for (int i = 0; i < CHUNK_SIZE; i++) {
		blocks[i] = grass;
	}
	blocksMode = BlocksMode::Default;
}
