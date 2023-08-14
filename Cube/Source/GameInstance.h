#pragma once

#include "Engine/EngineCore.h"

class World;
class PathtraceRenderer;

class GameInstance
{
public:

	GameInstance();

	void Init();

	void Tick(float deltaTime);

	forceinline World* GetWorld() const { return world; }

	void LoadAllBlocks();

private:

	void CreateAndInitializeOpenGLObjects();

	void RenderLoop();

private:

	World* world;
	PathtraceRenderer* pathtraceRenderer;

};

/**/
extern GameInstance* GetGameInstance();