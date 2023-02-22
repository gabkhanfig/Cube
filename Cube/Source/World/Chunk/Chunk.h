#pragma once

#include "ChunkDataTypes.h"
#include "ChunkRender.h"

class IBlock;
class ChunkRenderComponent;

class Chunk
{
private:

	/* Array of chunk blocks. */
	ChunkBlock* blocks;

	/**/
	ChunkRenderComponent* RenderComponent;

	/**/
	bool wasChunkModifiedThisTick;

public:

	/* Fills the entire chunk with air. */
	Chunk();

	~Chunk();

	/* Ticks this chunk and keeps track of if it needs to regenerate it's mesh. */
	void Tick(float deltaTime);

	/* Get a block at a specific relative position within the chunk. */
	IBlock* GetBlock(BlockPosition position);

	/* Get a reference to a block and it's chunk data at a specific relative position within the chunk. */
	ChunkBlock& ChunkBlockAt(BlockPosition position);

	/* Get a const reference to a block and it's chunk data at a specific relative position within the chunk. */
	const ChunkBlock& ChunkBlockAt(BlockPosition position) const;

private:



};