#pragma once

#include "Engine/EngineCore.h"

class World;

class GameInstance
{
public:

	GameInstance();

	void Init();

	void Tick(float deltaTime);

	forceinline World* GetWorld() const { return world; }

	void LoadAllBlocks();

private:

	World* world;

};

/**/
extern GameInstance* GetGameInstance();