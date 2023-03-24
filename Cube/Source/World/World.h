#pragma once

#include "../Engine/EngineCore.h"
#include "WorldTransform.h"
#include "Chunk/ChunkDataTypes.h"

class Player;
class Shader;
class VertexArrayObject;
class Chunk;
class ChunkRenderer;
class IBlock;

class World
{
public:

	World();

	void BeginWorld();

	void Tick(float deltaTime);

	Player* GetPlayer() const { return player; }

	/* Get a chunk at a specific chunk position. Returns nullptr if the chunk there doesn't exist. */
	Chunk* GetChunk(ChunkPosition position) const;

	/* Get a block at a specific world position. Returns nullptr if the chunk the block would be in doesn't exist. */
	IBlock* GetBlock(WorldPosition position) const;

	/* Get a chunk's block data at a specific world position. Returns nullptr if the chunk the block would be in doesn't exist. */
	ChunkBlock* ChunkBlockAt(WorldPosition position) const;

	/* Checks if a chunk is loaded. */
	bool DoesChunkExist(ChunkPosition position) const;

	/* Basically just checks if the chunk that the world position would be in exists. If it does, the block will always exist. */
	bool DoesBlockExist(WorldPosition position) const;

private:

	void DrawWorld();

private:

	Player* player;

	ChunkRenderer* chunkRenderer;

	HashMap<ChunkPosition, Chunk*> chunks;

};

World* GetWorld();