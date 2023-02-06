#pragma once

class Player;
class Shader;
class VertexArrayObject;

class World
{
private:

	Player* player;

	Shader* chunkShader;

	VertexArrayObject* chunkVAO;

public:

	World();

	void DrawWorld();


};

World* GetWorld();