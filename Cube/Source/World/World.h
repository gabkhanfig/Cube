#pragma once

#include "../Engine/EngineCore.h"
#include "WorldTransform.h"
#include "Chunk/ChunkDataTypes.h"
#include "Raycast.h"

class Player;
class Shader;
class VertexArrayObject;
class Chunk;
class ChunkRenderer;
class Block;
class TerrainGenerator;

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
	Block* GetBlock(WorldPosition position) const;

	/* Checks if a chunk is loaded. */
	bool DoesChunkExist(ChunkPosition position) const;

	/* Basically just checks if the chunk that the world position would be in exists. If it does, the block will always exist. */
	bool DoesBlockExist(WorldPosition position) const;

	ChunkRenderer* GetChunkRenderer() const { return chunkRenderer; }

	RaycastHitResult RaycastHit(const glm::dvec3 start, const glm::dvec3 end);

private:

	void DrawWorld();

	void CreateChunkRenderer();

	void RenderLoop();

private:

	Player* player;

	ChunkRenderer* chunkRenderer;

	HashMap<ChunkPosition, Chunk*> chunks;

	TerrainGenerator* terrainGenerator;

};

World* GetWorld();