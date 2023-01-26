#pragma once

#include <EngineCore.h>

void _CubeGameInstanceTickCallback(float DeltaTime);

class World;

class GameInstance
{
private:

	World* world;

public:

	void Init();

	void Tick(float DeltaTime);

	forceinline World* GetWorld() const { return world; }
};

/**/
extern GameInstance* GetGameInstance();