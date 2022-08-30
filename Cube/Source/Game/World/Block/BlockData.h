#pragma once

#include <CubeCore.h>

enum class Facing : uint8 
{
	Bottom,
	North,
	East,
	South,
	West,
	Top
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