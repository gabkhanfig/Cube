#pragma once

#include "Engine/EngineCore.h"

void _CubeGameInstanceTickCallback(float deltaTime);

class World;

class GameInstance
{
private:

	World* world;

	gk::ThreadPool* threadPool;

public:

	GameInstance();

	void Init();

	void Tick(float deltaTime);

	forceinline World* GetWorld() const { return world; }

	forceinline gk::ThreadPool* GetThreadPool() const { return threadPool; }
};

/**/
extern GameInstance* GetGameInstance();