#include "BlockFactory.h"

//#include "BlockTypes/Stone/StoneBlock.h"
//#include "BlockTypes/Air/AirBlock.h"

//std::unordered_map<GlobalString, BlockClass*> BlockFactory::blockClasses = MapBlockClasses();

//std::unordered_map<GlobalString, const BlockConstructionPair*> BlockFactory::blockClasses; 
//std::unordered_map<GlobalString, uint32> BlockFactory::materialIds;
//darray<Material> BlockFactory::materialsData;
//const BlockConstructionPair* BlockFactory::airFactory = nullptr;

darray<BlockTypeInfo> BlockFactory::allBlockTypeInfos;
std::unordered_map<GlobalString, uint32> BlockFactory::blockIds;

bool BlockFactory::IsValidBlockName(GlobalString blockName)
{
	return blockIds.contains(blockName);
}

Block BlockFactory::CreateBlock(const GlobalString blockName)
{
	gk_assertm(BlockFactory::IsValidBlockName(blockName), "Block \"" << blockName << "\" is not a valid block name");
	//const auto found = blockClasses.find(blockName);
	//const BlockConstructionPair* creator = found->second;
	//Block block = creator->blockClass->ClassDefaultBlock();
	//gk_assertm(block.vTable == nullptr, "Block's vtable from IBlockClass::ClassDefaultBlock() must not be set. Was set for block: " << blockName);
	//block.vTable = creator->vTable;
	//return block;
	return createBlock(blockIds.find(blockName)->second);
}

Block BlockFactory::createBlock(const uint32 id)
{
	Block block;
	block.data = 0;
	block.setBlockId(id);
	return block;
}

Block BlockFactory::CreateAirBlock()
{
	//gk_assertNotNull(airFactory);
	//Block air = airFactory->blockClass->ClassDefaultBlock();
	//gk_assertm(air.vTable == nullptr, "Air block's vtable from AirBlockClass::ClassDefaultBlock() must not be set.");
	//air.vTable = airFactory->vTable;
	//return air;
	return Block();
}

const BlockTypeInfo* BlockFactory::getBlockTypeInfo(const GlobalString blockName)
{
	gk_assertm(BlockFactory::IsValidBlockName(blockName), "Block \"" << blockName << "\" is not a valid block name");
	const uint32 id = blockIds.find(blockName)->second;
	gk_assertm(allBlockTypeInfos.Size() > id, "Block id " << id << " is not a valid block id");
	return &allBlockTypeInfos[id];
}

const BlockTypeInfo* BlockFactory::getBlockTypeInfo(const uint32 id)
{
	gk_assertm(allBlockTypeInfos.Size() > id, "Block id " << id << " is not a valid block id");
	return &allBlockTypeInfos[id];
}



