#pragma once

#include <CubeCore.h>
#include <Game/World/WorldPosition.h>
#include "ChunkBlocks.h"
#include "ChunkRender.h"

//#define CHUNK_WIDTH 16

#define CHUNK_SIZE CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_WIDTH

class Block;
struct Vertex;
class Dimension;

class Chunk 
{

public:

	//Block* blocks[CHUNK_SIZE];

	ChunkBlocks blocks;

	ChunkRender render;

	ChunkPosition position;

	//VertexBuffer* vertexBuffer;

	//IndexBuffer* indexBuffer;

	//VertexArray* vertexArray;

	Dimension* dimension;

	//uint32 quadsToDraw;

	//bool canRender;

public: 

	Chunk(Dimension* dim, ChunkPosition _position = {0, 0, 0});
	~Chunk();

	static Chunk* LoadChunk(Dimension* _dimension, ChunkPosition _position);

	void UnloadChunk();

	void GenerateFreshChunkTerrain();

	void DrawChunk();

	void Init();

	Block* GetBlockAtLocalPosition(BlockPosition bpos);

	void RegenerateChunkMeshData();

	glm::vec3 ShiftToRenderOrigin();

	bool IsChunkInCamera(glm::vec3& outRenderPos);
};