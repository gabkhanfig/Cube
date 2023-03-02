#pragma once

class Player;
class Shader;
class VertexArrayObject;
class Chunk;

class World
{
private:

	Player* player;

	Shader* chunkShader;

	VertexArrayObject* chunkVAO;

	Chunk* testChunk;

public:

	World();

	void DrawWorld();


};

World* GetWorld();