#pragma once

class Player;
class Shader;

class World
{
private:

	Player* player;

	Shader* chunkShader;

public:

	World();

	void DrawWorld();


};

World* GetWorld();