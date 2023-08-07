#pragma once

#include "../../Engine/EngineCore.h"
#include "BlockClass.h"
#include "Block.h"

class Chunk;
struct BlockVTable;

struct BlockFactory
{
public:

	template<typename T> 
		requires (std::is_base_of<IBlockClass, T>::value)
	static void AddBlock() {
		const IBlockClass* blockClass = new T();
		const BlockVTable* vTable = blockClass->NewBlockVTable();

#if CUBE_DEVELOPMENT
		AssertValidateVTable(blockClass->GetBlockName(), vTable);
#endif

		BlockConstructionPair* pair = new BlockConstructionPair();
		pair->blockClass = blockClass;
		pair->vTable = vTable;
		blockClasses.insert({ blockClass->GetBlockName(), pair });

		if constexpr (std::is_same_v<T, AirBlockClass>) {
			airFactory = pair;
		}
	}

	static bool IsValidBlock(const GlobalString blockName);

	static Block CreateBlock(const GlobalString blockName);

	static Block CreateAirBlock();

private:

	static void AssertValidateVTable(GlobalString blockName, const BlockVTable* vTable);

private:

	static std::unordered_map<GlobalString, const BlockConstructionPair*> blockClasses;
	static const BlockConstructionPair* airFactory;

};
