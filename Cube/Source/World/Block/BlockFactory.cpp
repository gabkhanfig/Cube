#include "BlockFactory.h"
#include "IBlock.h"

BlockClass::BlockClass()
	: defaultBlock(nullptr), classRef(nullptr), createNewBlock(false)
{}

IBlock* BlockClass::GetBlock()
{
	IBlock* block = nullptr;
	if (createNewBlock) {
		return (IBlock*)classRef->NewObject();
	}
	else {
		block = defaultBlock;
	}
	checkm(block != nullptr, "Block Dependency GetBlock() must never return nullptr");
	return block;
}

