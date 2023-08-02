#pragma once

#include "ChunkDataTypes.h"
//#include "ChunkRenderComponent.h"

class Block;
class ChunkRenderComponent;
class VertexArrayObject;
class Shader;
class ChunkRenderer;
class TerrainGenerator;

class Chunk
{
public:

	/* Fills the entire chunk with air. */
	Chunk(ChunkPosition inPosition);

	~Chunk();

	/* Ticks this chunk and keeps track of if it needs to regenerate it's mesh. */
	void Tick(float deltaTime);

	/* Get a block at a specific relative position within the chunk. */
	forceinline Block* GetBlock(BlockPosition position) const { return blocks[position.index]; }

	void SetBlockAt(BlockPosition position, Block* block);

	void DestroyBlockAt(BlockPosition position);

	/* Fill the chunk with a specified block. */
	void FillChunkWithBlock(GlobalString blockName);

	/* Must be thread-safe. */
	void GenerateTerrain(TerrainGenerator* terrainGenerator);

	forceinline ChunkPosition GetPosition() const { return position; }

	ChunkRenderComponent* GetRenderComponent() const { return renderComponent; }

	bool ShouldBeRemeshed() const { return shouldBeRemeshed; }

	void SetShouldBeRemeshed(bool newShouldBeRemeshed) { shouldBeRemeshed = newShouldBeRemeshed; }

	static void MultithreadGenerateTerrain(const darray<Chunk*>& chunks, gk::ThreadPool* threadPool, TerrainGenerator* terrainGenerator);

private:

	void DestroyAllBlocks();

private:

	const ChunkPosition position;

	/**/
	ChunkRenderComponent* renderComponent;

	/**/
	bool shouldBeRemeshed;

	/* Array of chunk blocks. */
	Block* blocks[CHUNK_SIZE];

};