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
#include "World/Render/PathtraceRenderer.h"
#include "Engine/OpenGL/OpenGLInstance.h"

GameInstance::GameInstance()
  : world(nullptr)
{
}

void GameInstance::Init()
{
  gk::Thread* renderThread = engine->GetRenderThread();
  if (!engine->IsUsingRenderThread()) {
    BlockTextureAtlas::CreateBlockTextureAtlasObject();
  }
  else {   
    renderThread->BindFunction(BlockTextureAtlas::CreateBlockTextureAtlasObject);
    renderThread->Execute();
    while (!renderThread->IsReady());
  }
  CubeInput::SetupGameCallbacks();

  LoadAllBlocks();

  renderThread->BindFunction(std::bind(&GameInstance::CreateAndInitializeOpenGLObjects, this));
  renderThread->Execute();
  while (!renderThread->IsReady());

  world = new World();
  world->BeginWorld();
}

void GameInstance::Tick(float deltaTime)
{
  CubeInput::Tick(deltaTime);
  world->Tick(deltaTime);
  RenderLoop();

  IObject::_DeleteAllPendingKillObjects();
}

void GameInstance::LoadAllBlocks()
{
  //cubeLog("loading all blocks");
  BlockFactory::AddBlock<AirBlockClass>();
  BlockFactory::AddBlock<StoneBlockClass>();
}

void GameInstance::CreateAndInitializeOpenGLObjects()
{
  assertOnRenderThread();
  pathtraceRenderer = new PathtraceRenderer();
}

void GameInstance::RenderLoop()
{
  gk::Thread* renderThread = engine->GetRenderThread();
  while (!renderThread->IsReady());
  world->RenderLoop();
  //renderThread->BindFunction(std::bind(&PathtraceRenderer::PerformTestDraw, pathtraceRenderer));
  //renderThread->Execute();

}
