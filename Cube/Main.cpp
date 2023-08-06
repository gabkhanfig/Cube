#include <glad/glad.h>
#include "Source/Engine/Engine.h"
#include <GLFW/glfw3.h>

#include "Source/Player/Player.h"
#include "Source/GameInstance.h"
#include "Source/Settings/Settings.h"

#include "Testing/CubeTest.h"

GameInstance* gameInstance;
Settings* settings;

GameInstance* GetGameInstance() {
  return gameInstance;
}

Settings* GetSettings() {
  return settings;
}

int main(int argc, char** argv)
{
#if RUN_TESTS
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
#else
  settings = new Settings();
  Engine::Start();
  gameInstance = new GameInstance();
  gameInstance->Init();
  Engine::Run(&_CubeGameInstanceTickCallback);
  return 0;
#endif
}