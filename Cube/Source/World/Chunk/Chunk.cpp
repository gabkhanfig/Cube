#include "Chunk.h"
#include "../Block/BlockFactory.h"
#include "../Block/Block.h"
#include "../Block/BlockTypes/Air/AirBlock.h"
#include "ChunkRenderComponent.h"
#include "../Terrain/TerrainGenerator.h"

Chunk::Chunk(ChunkPosition inPosition)
	: shouldBeRemeshed(false), position(inPosition)
{
	//blocks = new Block*[CHUNK_SIZE];
	renderComponent = new ChunkRenderComponent(this);

	//BlockClass* airBlockClass = BlockFactory::GetBlockClass(AirBlock::GetStaticName());
	//for (int i = 0; i < CHUNK_SIZE; i++) {
	//	//blocks[i] = airBlockClass->NewBlock();
	//	blocks[i] = nullptr;
	//}
}

Chunk::~Chunk()
{
	DestroyAllBlocks();
	//delete[] blocks;
	delete renderComponent;
}

void Chunk::Tick(float deltaTime)
{
}

void Chunk::SetBlockAt(BlockPosition position, const Block& block)
{
	blocks[position.index].Destroy();
	blocks[position.index] = block;
	SetShouldBeRemeshed(true);
}

void Chunk::DestroyBlockAt(BlockPosition position)
{
	blocks[position.index].Destroy();
	blocks[position.index] = BlockFactory::CreateAirBlock();
	SetShouldBeRemeshed(true);
}

void Chunk::FillChunkWithBlock(GlobalString blockName)
{
	for (int i = 0; i < CHUNK_SIZE; i++) {
		blocks[i].Destroy();
		blocks[i] = BlockFactory::CreateBlock(blockName);
	}
}

void Chunk::GenerateTerrain(TerrainGenerator* terrainGenerator)
{
	const WorldPosition initialPos = WorldPosition(position, BlockPosition());
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

static void GenerateTerrainTask(Chunk* chunk, TerrainGenerator* terrainGenerator) {
	chunk->GenerateTerrain(terrainGenerator);
	chunk->SetShouldBeRemeshed(true);
}

void Chunk::MultithreadGenerateTerrain(const darray<Chunk*>& chunks, gk::ThreadPool* threadPool, TerrainGenerator* terrainGenerator)
{
	for (Chunk* chunk : chunks) {
		threadPool->AddFunctionToQueue(std::bind(GenerateTerrainTask, chunk, terrainGenerator));
	}
	threadPool->ExecuteQueue();
}

void Chunk::DestroyAllBlocks()
{
	cubeLog("todo... Chunk::DestroyAllBlocks()");
	//for (int i = 0; i < CHUNK_SIZE; i++) {
	//	Block* block = blocks[i];
	//	if (IsValid(block)) {
	//		block->Destroy();
	//	}
	//}
}

