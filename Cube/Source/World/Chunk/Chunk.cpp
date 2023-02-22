#include "Chunk.h"
#include "../Block/BlockFactory.h"
#include "../Block/IBlock.h"
#include "ChunkRender.h"

Chunk::Chunk()
	: wasChunkModifiedThisTick(false)
{
	blocks = new ChunkBlock[CHUNK_SIZE];
	RenderComponent = new ChunkRenderComponent(this);
}

Chunk::~Chunk()
{
	delete[] blocks;
	delete RenderComponent;
}

void Chunk::Tick(float deltaTime)
{
	wasChunkModifiedThisTick = false;
}

IBlock* Chunk::GetBlock(BlockPosition position)
{
	const int blockIndex = position.ToBlockIndex();
	checkm(blockIndex < CHUNK_SIZE, "block index must be within CHUNK_SIZE");
	return blocks[blockIndex].GetBlock();
}

ChunkBlock& Chunk::ChunkBlockAt(BlockPosition position)
{
	const int blockIndex = position.ToBlockIndex();
	checkm(blockIndex < CHUNK_SIZE, "block index must be within CHUNK_SIZE");
	return blocks[blockIndex];
}

const ChunkBlock& Chunk::ChunkBlockAt(BlockPosition position) const
{
	const int blockIndex = position.ToBlockIndex();
	checkm(blockIndex < CHUNK_SIZE, "block index must be within CHUNK_SIZE");
	return blocks[blockIndex];
}

