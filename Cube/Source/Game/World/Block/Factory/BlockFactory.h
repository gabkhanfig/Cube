#pragma once
#include <CubeCore.h>
#include <type_traits>

class Block;
class SimpleBlock;
class ComplexBlock;

struct BlockInjectorPure 
{
	uint32 blockId; 

	/*
	Get a block object of this block.
	If it's a simple block, will be a pointer to that global instance of the block.
	If it's a complex block, will be a new object.
	*/
	virtual Block* GetBlockObject() = 0;
};

template <typename T>
struct BlockInjector : BlockInjectorPure
{
	static_assert(std::is_base_of<Block, T>::value, "Template block for block injector is not a base of Block!");

	static inline constexpr bool IsSimpleBlock() { return std::is_base_of<SimpleBlock, T>::value; }
	static inline constexpr bool IsComplexBlock() { return std::is_base_of<ComplexBlock, T>::value; }

	T* blockObj;

	static_assert(IsSimpleBlock() || IsComplexBlock(), "Template block for block injector is not a simple or complex block, thus it is invalid.");

	/*
	Default constructor. 
	If the template block is a simple block, 
	the pointer blockObj will be set to a valid new block, 
	which will be used as the block object wherever it exists.
	*/
	BlockInjector() {
		blockId = T::GetStaticBlockId();

		if (IsSimpleBlock()) {
			blockObj = new T();
		}
		else {
			blockObj = nullptr;
		}
	}

	Block* GetBlockObject() override
	{ 
		if (IsSimpleBlock()) {
			return blockObj;
		}
		else {
			return GetNewBlockObject();
		}
	}

	/* Get a new block object, regardless of simple or complex block behavior. */
	T* GetNewBlockObject() 
	{
		return new T();
	}

	static BlockInjectorPure* GetNewBlockInjector() { return static_cast<BlockInjectorPure*>(new BlockInjector<T>); }
};



class BlockFactory 
{
private:

	static std::unordered_map<uint32, BlockInjectorPure*> block_map;

public:

	/* Get a block object of this block.
	If it's a simple block, will be a pointer to that global instance of the block.
	If it's a complex block, will be a new object. */
	static Block* GetBlock(uint32 blockId);

	/* Inject a specific block. */
	template<class T>
	static void InjectBlock();

	/* Perform dependency injection for all blocks. */
	static void InjectAllBlocks();
};
