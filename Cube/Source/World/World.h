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
struct Block;
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
	Block* GetBlock(WorldPosition position);

	/* Get a block at a specific world position. Returns nullptr if the chunk the block would be in doesn't exist. */
	const Block* GetBlock(WorldPosition position) const;

	bool SetBlockAt(WorldPosition position, const Block& block);

	/* Destroys the block at the given position, replacing it with air. */
	void DestroyBlockAt(WorldPosition position);

	/* Checks if a chunk is loaded. */
	bool DoesChunkExist(ChunkPosition position) const;

	/* Basically just checks if the chunk that the world position would be in exists. If it does, the block will always exist. */
	bool DoesBlockExist(WorldPosition position) const;

	ChunkRenderer* GetChunkRenderer() const { return chunkRenderer; }

	RaycastHitResult RaycastHit(glm::dvec3 start, glm::dvec3 end) const;

private:

	void CreateChunkRenderer();

	void RenderLoop();

	/* Deletes chunks that are too far away, and  */
	void DeleteDistantChunksAndLoadNearby(int renderDistance);

	static darray<ChunkPosition> ChunkPositionsWithinRenderDistance(ChunkPosition center, int renderDistance);

private:

	Player* player;

	ChunkRenderer* chunkRenderer;

	HashMap<ChunkPosition, Chunk*> chunks;

	TerrainGenerator* terrainGenerator;

};

World* GetWorld();