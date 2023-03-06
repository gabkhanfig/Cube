#pragma once

#include "Engine/EngineCore.h"

void _CubeGameInstanceTickCallback(float deltaTime);

class World;

class GameInstance
{
private:

	World* world;

public:

	void Init();

	void Tick(float deltaTime);

	forceinline World* GetWorld() const { return world; }
};

/**/
extern GameInstance* GetGameInstance();