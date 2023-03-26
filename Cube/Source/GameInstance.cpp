#include "GameInstance.h"
#include "Engine/Engine.h"
#include "World/Block/BlockTextureAtlas.h"
#include "World/World.h"
#include "Engine/Input/UserInput.h"
#include "Input/CubeInput.h"
#include "Engine/Input/InputMapping.h"
#include "World/Block/BlockFactory.h"

void _CubeGameInstanceTickCallback(float deltaTime)
{
  GetGameInstance()->Tick(deltaTime);
}

GameInstance::GameInstance()
  : world(nullptr)
{
  threadPool = new gk::ThreadPool(gk::ThreadPool::SystemThreadCount() - 1);
}

void GameInstance::Init()
{
  BlockTextureAtlas::CreateBlockTextureAtlasObject();
  CubeInput::SetupGameCallbacks();
  world = new World();
  world->BeginWorld();
}

void GameInstance::Tick(float deltaTime)
{
  CubeInput::Tick(deltaTime);
  world->Tick(deltaTime);
}
