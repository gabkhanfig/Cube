#include "BlockFactory.h"
#include "Block.h"

#include "BlockTypes/Stone/StoneBlock.h"
#include "BlockTypes/Air/AirBlock.h"

template<typename B>
	requires (std::is_base_of<Block, B>::value)
static void AddBlockToMap(std::unordered_map<GlobalString, BlockClass*>* map) {
	BlockClass* block = B::GetStaticClass();
	map->insert({ block->GetName(), block });
}

static std::unordered_map<GlobalString, BlockClass*> MapBlockClasses() 
{
	std::unordered_map<GlobalString, BlockClass*> classes;

#define block(blockClass) AddBlockToMap<blockClass>(&classes)

	block(AirBlock);
	block(StoneBlock);

#undef block

	return classes;
}

BlockClass::BlockClass()
	: classRef(nullptr)
{}

Block* BlockClass::NewBlock()
{
	Block* block = (Block*)classRef->NewObject();
	checkm(block != nullptr, "Block Dependency GetBlock() must never return nullptr");
	return block;
}

std::unordered_map<GlobalString, BlockClass*> BlockFactory::blockClasses = MapBlockClasses();

BlockClass* BlockFactory::GetBlockClass(GlobalString blockName)
{
	auto found = blockClasses.find(blockName);
	if (found == blockClasses.end()) {
		std::cout << "Unable to find block class for block: " << blockName.ToString() << '\n';
		return nullptr;
	}
	return found->second;
}

Block* BlockFactory::NewAirBlock()
{
	static const GlobalString AirBlockName = "airBlock";
#ifdef DEVELOPMENT
	auto found = blockClasses.find(AirBlockName);
	checkm(found != blockClasses.end(), "Air block must be mapped in the block factory.");
	return found->second->NewBlock();
#else
	static IBlock* AirBlock = blockClasses.find(AirBlockName)->second->GetBlock();
	return AirBlock;
#endif
}
