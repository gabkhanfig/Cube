#include "Chunk.h"
#include "../Block/BlockFactory.h"
#include "../Block/IBlock.h"
#include "ChunkRenderComponent.h"

Chunk::Chunk()
	: wasChunkModifiedThisTick(false)
{
	blocks = new ChunkBlock[CHUNK_SIZE];
	renderComponent = new ChunkRenderComponent(this);
	position = ChunkPosition(0, 0, 0);
}

Chunk::~Chunk()
{
	delete[] blocks;
	delete renderComponent;
}

void Chunk::Tick(float deltaTime)
{
	wasChunkModifiedThisTick = false;
}

IBlock* Chunk::GetBlock(BlockPosition position) const
{
	const int blockIndex = position.index;
	checkm(blockIndex < CHUNK_SIZE, "block index must be within CHUNK_SIZE");
	return blocks[blockIndex].GetBlock();
}

ChunkBlock* Chunk::ChunkBlockAt(BlockPosition position) const
{
	const int blockIndex = position.index;
	checkm(blockIndex < CHUNK_SIZE, "block index must be within CHUNK_SIZE");
	return &blocks[blockIndex];
}

void Chunk::FillChunkWithBlock(GlobalString blockName)
{
	BlockClass* blockClass = BlockFactory::GetBlockClass(blockName);
	for (int i = 0; i < CHUNK_SIZE; i++) {
		IBlock* block = blockClass->GetBlock();
		blocks[i].ReplaceBlock(block);
	}
}

void Chunk::RecreateMesh() const
{
	renderComponent->RecreateMesh();
}

void Chunk::Draw(ChunkRenderer* renderer)
{
	renderComponent->Draw(renderer);
}

