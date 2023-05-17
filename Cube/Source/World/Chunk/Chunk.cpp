#include "Chunk.h"
#include "../Block/BlockFactory.h"
#include "../Block/Block.h"
#include "../Block/BlockTypes/Air/AirBlock.h"
#include "ChunkRenderComponent.h"
#include "../Terrain/TerrainGenerator.h"

Chunk::Chunk(ChunkPosition inPosition)
	: wasChunkModifiedThisTick(false), position(inPosition)
{
	blocks = new Block*[CHUNK_SIZE];
	renderComponent = new ChunkRenderComponent(this);

	BlockClass* airBlockClass = BlockFactory::GetBlockClass(AirBlock::GetStaticName());
	for (int i = 0; i < CHUNK_SIZE; i++) {
		//blocks[i] = airBlockClass->NewBlock();
		blocks[i] = nullptr;
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
	if (blockIndex > CHUNK_SIZE) {
		std::cout << blockIndex << std::endl;
	}
	checkm(blockIndex < CHUNK_SIZE, "block index must be within CHUNK_SIZE");
	return blocks[blockIndex];
}

void Chunk::FillChunkWithBlock(GlobalString blockName)
{
	BlockClass* blockClass = BlockFactory::GetBlockClass(blockName);
	for (int i = 0; i < CHUNK_SIZE; i++) {
		Block* block = blockClass->NewBlock();
		if (blocks[i]) {
			blocks[i]->Destroy();
		}
		blocks[i] = block;
		//blocks[i].ReplaceBlock(block);
	}
}

void Chunk::GenerateTerrain(TerrainGenerator* terrainGenerator)
{
	const WorldPosition initialPos = WorldPosition::FromChunkAndBlock(position, BlockPosition());
	int index = 0;
	for (int y = 0; y < CHUNK_LENGTH; y++) {
		for (int z = 0; z < CHUNK_LENGTH; z++) {
			for (int x = 0; x < CHUNK_LENGTH; x++) {
				blocks[index] = terrainGenerator->CreateBlockForWorldPosition(WorldPosition(initialPos.x + x, initialPos.y + y, initialPos.z + z));
				index++;
			}
		}
	}
}

