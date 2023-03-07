#pragma once

#include "../Engine/EngineCore.h"

class Player;
class Shader;
class VertexArrayObject;
class Chunk;
class ChunkRenderer;

class World
{
private:

	Player* player;

	Shader* chunkShader;

	VertexArrayObject* chunkVAO;

	Chunk* testChunk;

	ChunkRenderer* chunkRenderer;

public:

	World();

	void DrawWorld();

	Player* GetPlayer() const { return player; }

};

World* GetWorld();