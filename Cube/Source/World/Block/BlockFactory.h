#pragma once

#include <EngineCore.h>

class IBlock;
class Chunk;

class BlockClass
{
private:

	/**/
	GlobalString name;

	/**/
	IBlock* defaultBlock;
	
	/**/
	gk::ClassRef* classRef;

	/* Should new instances of this block be created. */
	bool createNewBlock;

public:

	BlockClass();

	template<typename B>
		requires (std::is_base_of<IBlock, B>::value)
	static BlockClass* CreateBlockDependency() 
	{
		BlockClass* blockDep = new BlockClass();
		blockDep->name = B::GetStaticName();

		const bool shouldCreateNewBlock = B::ShouldCreateNewBlock();
		if (!shouldCreateNewBlock) {
			blockDep->defaultBlock = new B();
		}
		blockDep->classRef = B::_GetStaticClassRef();
		blockDep->createNewBlock = shouldCreateNewBlock;

		return blockDep;
	}

	/* Either gets the default block object, or creates a new one depending on the block's requirements. */
	IBlock* GetBlock();

	GlobalString GetName() const { return name; }
};

class BlockFactory
{
private:

	static std::unordered_map<GlobalString, BlockClass*> blockClasses;

public:

	static BlockClass* GetBlockClass(GlobalString blockName);

	static IBlock* GetAirBlock();

};

