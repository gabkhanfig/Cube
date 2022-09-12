#pragma once

#include <CubeCore.h>
#include <Game/World/WorldPosition.h>

enum class Facing : uint8 
{
	Down,
	North,
	East,
	South,
	West,
	Up
};

struct BlockTransform 
{
	BlockPosition position;
	uint8 horizontalFacing : 2;
	uint8 verticalFacing : 1;


	Facing GetHorizontalDirection() 
	{
		switch (horizontalFacing) {
		case 0:
			return Facing::North;
		case 1:
			return Facing::East;
		case 2:
			return Facing::South;
		case 3:
			return Facing::West;
		}
	}

	Facing GetVerticalDirection() 
	{
		switch (verticalFacing) {
		case 0:
			return Facing::Down;
		case 1:
			return Facing::Up;
		}
	}
};

struct BlockData 
{
	uint8 horizontalDirection : 2;
	uint8 verticalDirection : 1;

	class Block* block;
};

#define BLOCK_BODY(blockIdNumber, blockName) \
private: \
static const uint32 blockId = blockIdNumber; \
public: \
static inline uint32 GetStaticBlockId() { return blockId; } \
static inline std::string GetStaticBlockName() { return blockName; } \
virtual inline uint32 GetBlockId() override { return GetStaticBlockId(); } \
virtual inline std::string GetBlockName() override { return GetStaticBlockName(); }