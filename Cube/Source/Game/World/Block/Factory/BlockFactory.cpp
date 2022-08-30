#include "BlockFactory.h"
#include <Game/World/Block/Block.h>
#include <Game/World/Block/Blocks/Air/Air.h>
#include <Game/World/Block/Blocks/Dirt/Dirt.h>
#include <Game/World/Block/Blocks/GrassBlock/Grass.h>
#include <Game/World/Block/Blocks/Stone/Stone.h>
#include <Game/World/Block/Blocks/StonyGrass/StonyGrass.h>

std::unordered_map<uint32, BlockInjectorPure*> BlockFactory::block_map;

Block* BlockFactory::GetBlock(uint32 blockId)
{
	check(block_map.find(blockId) != block_map.end());

	Block* block = block_map[blockId]->GetBlockObject();
	return block;
}

template<class T>
void BlockFactory::InjectBlock()
{
	checkLog(block_map.find(T::GetStaticBlockId()) == block_map.end(), "Block already exists in block factory");

	block_map.insert({ T::GetStaticBlockId(), BlockInjector<T>::GetNewBlockInjector() });
}

void BlockFactory::InjectAllBlocks()
{
	InjectBlock<Air>();
	InjectBlock<Dirt>();
	InjectBlock<Grass>();
	InjectBlock<Stone>();
	InjectBlock<StonyGrass>();
}

