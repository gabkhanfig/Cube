#include "GameInstance.h"
#include "Engine/Engine.h"
#include "World/Block/BlockTextureAtlas.h"
#include "World/World.h"
#include "Engine/Input/UserInput.h"
#include "Input/CubeInput.h"
#include "Engine/Input/InputMapping.h"
#include "World/Block/BlockFactory.h"
#include "Settings/Settings.h"

#include "World/Block/BlockClass.h"

GameInstance::GameInstance()
  : world(nullptr)
{
}

void GameInstance::Init()
{
  if (!engine->IsUsingRenderThread()) {
    BlockTextureAtlas::CreateBlockTextureAtlasObject();
  }
  else {
    gk::Thread* renderThread = engine->GetRenderThread();
    renderThread->BindFunction(BlockTextureAtlas::CreateBlockTextureAtlasObject);
    renderThread->Execute();
    while (!renderThread->IsReady());
  }
  CubeInput::SetupGameCallbacks();

  LoadAllBlocks();

  world = new World();
  world->BeginWorld();
}

void GameInstance::Tick(float deltaTime)
{
  CubeInput::Tick(deltaTime);
  world->Tick(deltaTime);

  IObject::_DeleteAllPendingKillObjects();
}

void GameInstance::LoadAllBlocks()
{
  //cubeLog("loading all blocks");
  BlockFactory::AddBlock<AirBlockClass>();
  BlockFactory::AddBlock<StoneBlockClass>();
}
