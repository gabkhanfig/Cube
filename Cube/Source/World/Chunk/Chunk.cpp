#include "Chunk.h"
#include "../Block/BlockFactory.h"
#include "../Block/Block.h"
#include "../Block/BlockTypes/Air/AirBlock.h"
#include "ChunkRenderComponent.h"

Chunk::Chunk(ChunkPosition inPosition)
	: wasChunkModifiedThisTick(false), position(inPosition)
{
	blocks = new Block*[CHUNK_SIZE];
	renderComponent = new ChunkRenderComponent(this);

	BlockClass* airBlockClass = BlockFactory::GetBlockClass(AirBlock::GetStaticName());
	for (int i = 0; i < CHUNK_SIZE; i++) {
		blocks[i] = airBlockClass->GetBlock();
	}
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

Block* Chunk::GetBlock(BlockPosition position) const
{
	const int blockIndex = position.index;
	checkm(blockIndex < CHUNK_SIZE, "block index must be within CHUNK_SIZE");
	return blocks[blockIndex];
}

void Chunk::FillChunkWithBlock(GlobalString blockName)
{
	BlockClass* blockClass = BlockFactory::GetBlockClass(blockName);
	for (int i = 0; i < CHUNK_SIZE; i++) {
		Block* block = blockClass->GetBlock();
		if (blocks[i]) {
			blocks[i]->Destroy();
		}
		blocks[i] = block;
		//blocks[i].ReplaceBlock(block);
	}
}

void Chunk::RecreateMesh() const
{
	ChunkMesh* mesh = renderComponent->GetMesh();
	renderComponent->RecreateMesh(mesh);
}

