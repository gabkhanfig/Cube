#include "BlockFactory.h"

//#include "BlockTypes/Stone/StoneBlock.h"
//#include "BlockTypes/Air/AirBlock.h"

//std::unordered_map<GlobalString, BlockClass*> BlockFactory::blockClasses = MapBlockClasses();

std::unordered_map<GlobalString, const BlockCreatorPair*> BlockFactory::blockClasses; 
const BlockCreatorPair* BlockFactory::airFactory = nullptr;

bool BlockFactory::IsValidBlock(GlobalString blockName)
{
	return blockClasses.contains(blockName);
}

Block BlockFactory::CreateBlock(const GlobalString blockName)
{
	gk_assertm(BlockFactory::IsValidBlock(blockName), "Block \"" << blockName << "\" is not a valid block name");
	const auto found = blockClasses.find(blockName);
	const BlockCreatorPair* creator = found->second;
	Block block = creator->blockClass->ClassDefaultBlock();
	gk_assertm(block.vTable == nullptr, "Block's vtable from IBlockClass::ClassDefaultBlock() must not be set. Was set for block: " << blockName);
	block.vTable = creator->vTable;
	return block;
}

Block BlockFactory::CreateAirBlock()
{
	gk_assertNotNull(airFactory);
	Block air = airFactory->blockClass->ClassDefaultBlock();
	gk_assertm(air.vTable == nullptr, "Air block's vtable from AirBlockClass::ClassDefaultBlock() must not be set.");
	air.vTable = airFactory->vTable;
	return air;
}

void BlockFactory::AssertValidateVTable(GlobalString blockName, const BlockVTable* vTable)
{
	gk_assertm(vTable->blockName != GlobalString(), "Block vTable name must be set. Failed on block: " << blockName);
	gk_assertm(vTable->blockName == blockName, "Block vTable name must match block's name. Failed on block: " << blockName);
	gk_assertm(vTable->meshFunc != nullptr, "Block vTable mesh function must be set. Failed on block: " << blockName);
	gk_assertm(vTable->getFaceTexture != nullptr, "Block vTable get face texture function must be set. Failed on block: " << blockName);
}



