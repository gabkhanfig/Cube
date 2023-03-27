#pragma once

#include "Engine/EngineCore.h"

void _CubeGameInstanceTickCallback(float deltaTime);

class World;

class GameInstance
{
public:

	GameInstance();

	void Init();

	void Tick(float deltaTime);

	forceinline World* GetWorld() const { return world; }

	forceinline gk::ThreadPool* GetThreadPool() const { return threadPool; }

private:

	World* world;

	gk::ThreadPool* threadPool;
};

/**/
extern GameInstance* GetGameInstance();