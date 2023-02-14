#include "BlockFactory.h"
#include "IBlock.h"

#include "BlockTypes/Stone/StoneBlock.h"

template<typename B>
	requires (std::is_base_of<IBlock, B>::value)
static void AddBlockToMap(std::unordered_map<GlobalString, BlockClass*>* map) {
	BlockClass* block = B::GetStaticClass();
	map->insert({ block->GetName(), block });
}

static std::unordered_map<GlobalString, BlockClass*> MapBlockClasses() 
{
	std::unordered_map<GlobalString, BlockClass*> classes;

#define block(blockClass) AddBlockToMap<blockClass>(&classes)

	block(StoneBlock);

	return classes;
}

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
