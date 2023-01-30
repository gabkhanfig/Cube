#include "GameInstance.h"
#include "Engine.h"
#include "World/Block/BlockTextureAtlas.h"
#include "World/World.h"
#include <Input/UserInput.h>
#include "Input/CubeInput.h"
#include <Input/InputMapping.h>

void _CubeGameInstanceTickCallback(float deltaTime)
{
  GetGameInstance()->Tick(deltaTime);
}

void GameInstance::Init()
{
  BlockTextureAtlas::CreateBlockTextureAtlasObject();
  CubeInput::SetupGameCallbacks();
  world = new World();
}

void GameInstance::Tick(float deltaTime)
{
  CubeInput::Tick(deltaTime);
  world->DrawWorld();
}
