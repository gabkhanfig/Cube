#pragma once

#include <CubeCore.h>
#include <Game/World/WorldPosition.h>
#include <Engine/Render/Buffers/IndexBuffer.h>
#include <Engine/Render/Buffers/VertexBuffer.h>
#include <Engine/Render/Buffers/VertexArray.h>
#include <Engine/Render/Buffers/VertexBufferLayout.h>
#include <thread>

//#define CHUNK_WIDTH 16

#define CHUNK_SIZE CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_WIDTH

class Block;
struct Vertex;

class Chunk 
{

public:

	ChunkPosition position;

	Block* blocks[CHUNK_SIZE];

	uint32 quadsToDraw;
	bool canRender;
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;
	VertexArray* vertexArray;

	//std::thread RenderThread;

public: 

	Chunk(ChunkPosition _position = {0, 0, 0});
	~Chunk();

	void DrawChunk();

	void Init();

	Block* GetBlockAtLocalPosition(BlockPosition position);

	BlockPosition BlockIndexToRelativeLocation(uint32 index);

	void RegenerateChunkMeshData();

	glm::vec3 ShiftToRenderOrigin();
};