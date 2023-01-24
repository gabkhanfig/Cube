#include "BlockFactory.h"

#include "Block.h"

BlockClass::BlockClass()
	: defaultBlock(nullptr), classRef(nullptr), createNewBlock(false)
{}

Block* BlockClass::GetBlock()
{
	Block* block = nullptr;
	if (createNewBlock) {
		return (Block*)classRef->NewObject();
	}
	else {
		block = defaultBlock;
	}
	checkm(block != nullptr, "Block Dependency GetBlock() must never return nullptr");
	return block;
}

