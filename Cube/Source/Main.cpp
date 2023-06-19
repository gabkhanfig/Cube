#include <glad/glad.h>
#include "Engine/Engine.h"
#include <GLFW/glfw3.h>

#include "World/Block/Block.h"
#include "World/Block/BlockTextureAtlas.h"
#include "Player/Player.h"
#include "GameInstance.h"
#include "Settings/Settings.h"

GameInstance* gameInstance;
Settings* settings;

GameInstance* GetGameInstance() {
  return gameInstance;
}

Settings* GetSettings() {
  return settings;
}

int main(void)
{
  settings = new Settings();
  Engine::Start();
  gameInstance = new GameInstance();
  gameInstance->Init();
  Engine::Run(&_CubeGameInstanceTickCallback);
  return 0;
}