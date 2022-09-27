#pragma once

#include <CubeCore.h>
#include <Game/World/WorldPosition.h>

class Block;

struct ChunkBlocks 
{
	enum class BlocksMode : uint8
	{
		/**/
		Default,

		/**/
		AirFilled,

		/**/

	};

private:

	Block* airBlock;

	Block** blocks;

	BlocksMode blocksMode;

public:

	ChunkBlocks();

	~ChunkBlocks();

	/* Get the block at a specified index. */
	Block* GetBlock(int index);

	/* Get the block at a specified relative position. */
	Block* GetBlock(BlockPosition pos);

	/* Get block with index of operator using array index. */
	Block* operator [] (int index) {
		return GetBlock(index);
	}

	/* Get block with index of operator using block relative position. */
	Block* operator [] (BlockPosition pos) {
		return GetBlock(pos);
	}
	
	bool CheckIfShouldBeAirFilled();

	void ClearBlocks();

	void TestSetAllBlocks();

	BlocksMode GetBlocksMode() { return blocksMode; }
};

