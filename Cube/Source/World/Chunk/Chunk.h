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
private:

	/* Array of chunk blocks. */
	Block** blocks;

	const ChunkPosition position;

	/**/
	ChunkRenderComponent* renderComponent;

	/**/
	bool shouldBeRemeshed;

public:

	/* Fills the entire chunk with air. */
	Chunk(ChunkPosition inPosition);

	~Chunk();

	/* Ticks this chunk and keeps track of if it needs to regenerate it's mesh. */
	void Tick(float deltaTime);

	/* Get a block at a specific relative position within the chunk. */
	Block* GetBlock(BlockPosition position) const;

	void SetBlockAt(BlockPosition position, Block* block);

	void DestroyBlockAt(BlockPosition position);

	/* Fill the chunk with a specified block. */
	void FillChunkWithBlock(GlobalString blockName);

	void GenerateTerrain(TerrainGenerator* terrainGenerator);

	forceinline ChunkPosition GetPosition() const { return position; }

	ChunkRenderComponent* GetRenderComponent() const { return renderComponent; }

	bool ShouldBeRemeshed() const { return shouldBeRemeshed; }

	void SetShouldBeRemeshed(bool newShouldBeRemeshed) { shouldBeRemeshed = newShouldBeRemeshed; }

private:

	void DestroyAllBlocks();

};