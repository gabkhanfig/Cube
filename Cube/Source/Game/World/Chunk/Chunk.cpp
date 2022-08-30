#include "Chunk.h"
#include <Game/World/Block/Block.h>
#include <Engine/Render/Renderer.h>
#include <Engine/Render/Shader/Shader.h>
#include <Util/Benchmark/Benchmark.h>
#include <Game/World/Block/Factory/BlockFactory.h>

Chunk::Chunk(ChunkPosition _position)
{
	//Benchmark* chunkLoad = Benchmark::StartBenchmark("Construct chunk");

	indexBuffer = nullptr;
	vertexBuffer = nullptr;
	vertexArray = nullptr;

	canRender = false;

	position = _position;

	Print("chunk spawned at chunk position: " + ToString({ position.x, position.y, position.z }));
	//chunkLoad->EndBenchmark();

	int blockIndex = 0;
	Block* block = BlockFactory::GetBlock(BlockId_StonyGrass);
	Block* block2 = BlockFactory::GetBlock(BlockId_Air);
	for (uint8 y = 0; y < CHUNK_WIDTH; y++) {
		for (uint8 z = 0; z < CHUNK_WIDTH; z++) {
			for (uint8 x = 0; x < CHUNK_WIDTH; x++) {
				//Print("Block index: " + ToString(blockIndex));
				if (y >= 8) {
					blocks[blockIndex] = block2;
				}
				else {
					blocks[blockIndex] = block;
				}
				blockIndex++;
			}
		}
	}
}

Chunk::~Chunk()
{
	delete vertexArray;
	delete vertexBuffer;
	delete indexBuffer;
	for (int i = 0; i < CHUNK_SIZE; i++) {
		delete blocks[i];
	}
}

void Chunk::DrawChunk()
{
	if (canRender == false) return;

#ifdef DEVELOPMENT
	if (!vertexArray) return;
	if (!vertexBuffer) return;
	if (!indexBuffer) return;
#endif

	vertexArray->Bind();
	vertexBuffer->Bind();
	indexBuffer->Bind();

	Shader::GetBoundShader()->SetUniform3float("u_ChunkPosition", glm::vec3(position.x * CHUNK_WIDTH, position.y * CHUNK_WIDTH, position.z * CHUNK_WIDTH));
	Renderer::DrawQuads(quadsToDraw);
}

void Chunk::Init()
{
	//int blockIndex = 0;
	//for (uint8 y = 0; y < CHUNK_WIDTH; y++) {
	//	for (uint8 z = 0; z < CHUNK_WIDTH; z++) {
	//		for (uint8 x = 0; x < CHUNK_WIDTH; x++) {
	//			blocks[blockIndex] = Block::GetSingletonTest();
	//			blockIndex++;
	//		}
	//	}
	//}

	RegenerateChunkMeshData();
}

Block* Chunk::GetBlockAtLocalPosition(BlockPosition position)
{
	if (position.x >= CHUNK_WIDTH || position.y >= CHUNK_WIDTH || position.z >= CHUNK_WIDTH) {
		//std::cout << "position data is out of chunk range ... is null" << '\n';
		return nullptr;
	}

	uint32 index = 0;
	index += position.x;
	index += position.z * CHUNK_WIDTH;
	index += position.y * CHUNK_WIDTH * CHUNK_WIDTH;

	if (index >= CHUNK_SIZE) {
		//std::cout << "getting block at index " << index << "... is null" << '\n';
		return nullptr;
	}
	//std::cout << "getting block at index " << index << "... is valid!" << '\n';
	return blocks[index];
}

BlockPosition Chunk::BlockIndexToRelativeLocation(uint32 index)
{
#ifdef DEVELOPMENT
	if (index >= CHUNK_SIZE) {
		Print("Chunk indexing for block relative location of " + ToString(index) + " exceeds the maximum index of " + ToString(CHUNK_SIZE));
		return { 0, 0, 0 };
	}
#endif

	BlockPosition pos;
	pos.x = index % CHUNK_WIDTH;
	pos.y = index / (CHUNK_WIDTH * CHUNK_WIDTH); // integer flooring
	pos.z = (index % (CHUNK_WIDTH * CHUNK_WIDTH)) / CHUNK_WIDTH; // compress index into range 0-255, then integer division
	return pos;
}

void Chunk::RegenerateChunkMeshData()
{
	if (indexBuffer) {
		delete indexBuffer;
	}
	if (vertexBuffer) {
		delete vertexBuffer;
	}
	if (vertexArray) {
		delete vertexArray;
	}

	std::vector<BlockQuad> quads;

	for (int i = 0; i < CHUNK_SIZE; i++) {
		Block* block = blocks[i];

		BlockQuad quad;

		BlockPosition relativeLocation = BlockIndexToRelativeLocation(i);
		WorldPosition worldPos = {
			position.x * 16 + relativeLocation.x,
			position.y * 16 + relativeLocation.y,
			position.z * 16 + relativeLocation.z
		};

		if (block->IsBlockAdjacentTransparent(this, worldPos, Facing::Bottom)) {
			block->GenerateQuadDataForFace(this, relativeLocation, Facing::Bottom, quad);
			quads.push_back(quad);
		}
		if (block->IsBlockAdjacentTransparent(this, worldPos, Facing::North)) {
			block->GenerateQuadDataForFace(this, relativeLocation, Facing::North, quad);
			quads.push_back(quad);
		}
		if (block->IsBlockAdjacentTransparent(this, worldPos, Facing::East)) {
			block->GenerateQuadDataForFace(this, relativeLocation, Facing::East, quad);
			quads.push_back(quad);
		}
		if (block->IsBlockAdjacentTransparent(this, worldPos, Facing::South)) {
			block->GenerateQuadDataForFace(this, relativeLocation, Facing::South, quad);
			quads.push_back(quad);
		}
		if (block->IsBlockAdjacentTransparent(this, worldPos, Facing::West)) {
			block->GenerateQuadDataForFace(this, relativeLocation, Facing::West, quad);
			quads.push_back(quad);
		}
		if (block->IsBlockAdjacentTransparent(this, worldPos, Facing::Top)) {
			block->GenerateQuadDataForFace(this, relativeLocation, Facing::Top, quad);
			quads.push_back(quad);
		}
	}

	if (quads.size() == 0) {
		canRender = false;
		return;
	}

	vertexArray = new VertexArray();
	vertexBuffer = new VertexBuffer(&quads[0], sizeof(BlockQuad) * quads.size());  

#ifdef COMPRESSED_RENDER
	VertexBufferLayout layout = VertexBufferLayout::MakeVertexBufferLayoutForBlock();
#else
	VertexBufferLayout layout = VertexBufferLayout::MakeVertexBufferLayoutForVertex();
#endif
	vertexArray->AddBuffer(vertexBuffer, layout);

	indexBuffer = IndexBuffer::MakeQuadsIndexBuffer(quads.size());

	quadsToDraw = quads.size();
	canRender = true;
}
