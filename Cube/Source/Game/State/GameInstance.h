#pragma once

#include <CubeCore.h>

class GameInstance 
{
private:

	GameInstance();

	static GameInstance* gameInstance;


public:

	static GameInstance* StartGame();

	static GameInstance* GetGameInstance() { return gameInstance; }

	void Init();

	void Tick(float deltaTime);
};