#include <glad/glad.h>
#include "Engine/Engine.h"
#include <GLFW/glfw3.h>

#include "World/Block/Block.h"
#include "World/Block/BlockTextureAtlas.h"
#include "Player/Player.h"
#include "GameInstance.h"

GameInstance* gameInstance;

GameInstance* GetGameInstance() {
  return gameInstance;
}

int main(void)
{
  Engine::Start();
  gameInstance = new GameInstance();
  gameInstance->Init();
  Engine::Run(&_CubeGameInstanceTickCallback);
  return 0;
}