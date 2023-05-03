#pragma once

#include "ChunkDataTypes.h"
//#include "ChunkRenderComponent.h"

class Block;
class ChunkRenderComponent;
class VertexArrayObject;
class Shader;
class ChunkRenderer;

class Chunk
{
private:

	/* Array of chunk blocks. */
	ChunkBlock* blocks;

	ChunkPosition position;

	/**/
	ChunkRenderComponent* renderComponent;

	/**/
	bool wasChunkModifiedThisTick;

public:

	/* Fills the entire chunk with air. */
	Chunk(ChunkPosition inPosition);

	~Chunk();

	/* Ticks this chunk and keeps track of if it needs to regenerate it's mesh. */
	void Tick(float deltaTime);

	/* Get a block at a specific relative position within the chunk. */
	Block* GetBlock(BlockPosition position) const;

	/* Get a reference to a block and it's chunk data at a specific relative position within the chunk. */
	ChunkBlock* ChunkBlockAt(BlockPosition position) const;

	/* Fill the chunk with a specified block. */
	void FillChunkWithBlock(GlobalString blockName);

	forceinline ChunkPosition GetPosition() const { return position; }

	void RecreateMesh() const;

	ChunkRenderComponent* GetRenderComponent() const { return renderComponent; }

private:



};