#pragma once

#include <EngineCore.h>

class Block;
class Chunk;

class BlockClass
{
private:

	/**/
	GlobalString name;

	/**/
	Block* defaultBlock;
	
	/**/
	gk::ClassRef* classRef;

	/* Should new instances of this block be created. */
	bool createNewBlock;

public:

	BlockClass();

	template<typename B>
		requires (std::is_base_of<Block, B>::value)
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
	Block* GetBlock();

	GlobalString GetName() const { return name; }
};


