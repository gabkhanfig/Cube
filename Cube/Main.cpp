#include <glad/glad.h>
#include "Source/Engine/Engine.h"
#include <GLFW/glfw3.h>

#include "Source/World/Block/Block.h"
#include "Source/World/Block/BlockTextureAtlas.h"
#include "Source/Player/Player.h"
#include "Source/GameInstance.h"
#include "Source/Settings/Settings.h"

#include "Source/World/Chunk/BuriedChunkBlocks.h"

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
#if RUN_ALL_CUBE_TESTS
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
#else
  //BuriedChunkBlocks b;
  //const uint64* arr = b.GetBitmaskAsIntArray();
  //for (int i = 0; i < 8; i++) {
  //  if (arr[i] != 0)
  //    std::cout << "index: " << i << " is not set to 0" << std::endl;
  //}
  //std::cout << "done" << std::endl;


  settings = new Settings();
  Engine::Start();
  gameInstance = new GameInstance();
  gameInstance->Init();
  Engine::Run(&_CubeGameInstanceTickCallback);
  return 0;
#endif
}