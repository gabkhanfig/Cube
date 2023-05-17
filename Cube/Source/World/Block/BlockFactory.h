#pragma once

#include "../../Engine/EngineCore.h"

class Block;
class Chunk;

class BlockClass
{
private:

	/**/
	GlobalString name;
	
	/**/
	gk::ClassRef* classRef;

public:

	BlockClass();

	template<typename B>
		requires (std::is_base_of<Block, B>::value)
	static BlockClass* CreateBlockDependency() 
	{
		BlockClass* blockDep = new BlockClass();
		blockDep->name = B::GetStaticName();
		blockDep->classRef = B::_GetStaticClassRef();

		return blockDep;
	}

	/* Either gets the default block object, or creates a new one depending on the block's requirements. */
	Block* NewBlock();

	GlobalString GetName() const { return name; }
};

class BlockFactory
{
private:

	static std::unordered_map<GlobalString, BlockClass*> blockClasses;

public:

	static BlockClass* GetBlockClass(GlobalString blockName);

	static Block* NewAirBlock();

};

