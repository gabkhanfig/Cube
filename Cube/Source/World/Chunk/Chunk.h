#pragma once

#include "ChunkDataTypes.h"
//#include "ChunkRenderComponent.h"

class IBlock;
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

	/* Fill the chunk with a specified block. */
	void FillChunkWithBlock(GlobalString blockName);

	forceinline ChunkPosition GetPosition() const { return position; }

	void RecreateMesh() const;

	void Draw(ChunkRenderer* renderer);

	ChunkRenderComponent* GetRenderComponent() const { return renderComponent; }

private:



};