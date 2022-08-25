#include "Chunk.h"
#include <Game/World/Block/Block.h>
#include <Engine/Render/Renderer.h>

Chunk::Chunk(ChunkPosition _position)
{
	indexBuffer = nullptr;
	vertexBuffer = nullptr;

	canRender = false;

	position = _position;

	uint8 xcoord = 0;
	uint8 ycoord = 0;
	uint8 zcoord = 0;

	int blockIndex = 0;
	for (uint8 y = 0; y < CHUNK_HEIGHT; y++) {
		for (uint8 z = 0; z < CHUNK_WIDTH; z++) {
			for (uint8 x = 0; x < CHUNK_WIDTH; x++) {
				Block* stone = new Block();
				stone->chunk = this;
				stone->localPosition = { x, y, z };
				blocks[blockIndex] = stone;
				//std::cout << "block at index " << blockIndex << ": " << int(x) << " " << int(y) << " " << int(z) << '\n';
				blockIndex++;
			}
		}
	}

	RegenerateChunkMeshData();
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
	vertexArray->Bind();
	vertexBuffer->Bind();
	indexBuffer->Bind();

	Renderer::DrawQuads(quadsToDraw);
}

Block* Chunk::GetBlockAtLocalPosition(BlockPosition position)
{
	if (position.x >= CHUNK_WIDTH || position.y >= CHUNK_HEIGHT || position.z >= CHUNK_WIDTH) {
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

	std::vector<Quad> quads;

	for (int i = 0; i < CHUNK_SIZE; i++) {
		Block* block = blocks[i];
		Quad quad;

		if (block->IsBlockAdjacentTransparent(BlockFace::Bottom)) {
			Quad quad;
			block->GenerateQuadDataForFace(BlockFace::Bottom, quad);
			quads.push_back(quad);
		}
		if (block->IsBlockAdjacentTransparent(BlockFace::North)) {
			block->GenerateQuadDataForFace(BlockFace::North, quad);
			quads.push_back(quad);
		}
		if (block->IsBlockAdjacentTransparent(BlockFace::East)) {
			block->GenerateQuadDataForFace(BlockFace::East, quad);
			quads.push_back(quad);
		}
		if (block->IsBlockAdjacentTransparent(BlockFace::South)) {
			block->GenerateQuadDataForFace(BlockFace::South, quad);
			quads.push_back(quad);
		}
		if (block->IsBlockAdjacentTransparent(BlockFace::West)) {
			block->GenerateQuadDataForFace(BlockFace::West, quad);
			quads.push_back(quad);
		}
		if (block->IsBlockAdjacentTransparent(BlockFace::Top)) {
			block->GenerateQuadDataForFace(BlockFace::Top, quad);
			quads.push_back(quad);
		}
	}

	vertexArray = new VertexArray();
	vertexBuffer = new VertexBuffer(&quads[0], sizeof(Quad) * quads.size());  
	VertexBufferLayout layout = VertexBufferLayout::MakeVertexBufferLayoutForVertex();
	vertexArray->AddBuffer(vertexBuffer, layout);

	const uint32 indicesSize = quads.size() * 6;
	uint32* indices = new uint32[indicesSize];

	uint32 index = 0;
	for (uint32 i = 0; i < quads.size(); i++) {
		indices[index++] = ((i * 4));
		indices[index++] = (1 + (i * 4));
		indices[index++] = (2 + (i * 4));
		indices[index++] = (2 + (i * 4));
		indices[index++] = (3 + (i * 4));
		indices[index++] = (0 + (i * 4));
	}

	indexBuffer = new IndexBuffer(&indices[0], indicesSize);

	quadsToDraw = quads.size();
}
