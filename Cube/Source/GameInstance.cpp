#include "GameInstance.h"
#include "Engine.h"
#include "World/Block/BlockTextureAtlas.h"
#include "World/World.h"

void _CubeGameInstanceTickCallback(float DeltaTime)
{
  GetGameInstance()->Tick(DeltaTime);
}

void GameInstance::Init()
{
  BlockTextureAtlas::CreateBlockTextureAtlasObject();
  world = new World();
}

void GameInstance::Tick(float DeltaTime)
{
  world->DrawWorld();
}
