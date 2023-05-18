#include <glad/glad.h>
#include "Engine/Engine.h"
//#include <Window/Window.h>
//#include <Graphics/Shader/Shader.h>
//#include <Graphics/Buffers/VertexBufferObject.h>
//#include <Graphics/Buffers/IndexBufferObject.h>
//#include <Graphics/Buffers/VertexArrayObject.h>
//#include <Graphics/Buffers/ShaderBufferObject.h>
//#include "../Resources/GeneratedFiles.h"
//#include <algorithm>
//#include <Graphics/Texture/Texture2d.h>
//#include <Input/UserInput.h>
#include <GLFW/glfw3.h>

#include "World/Block/Block.h"
//#include "Vendor/stb_image/stb_image.h"
#include "World/Block/BlockTextureAtlas.h"
#include "Player/Player.h"
//#include <Graphics/Camera/Camera.h>
#include "GameInstance.h"

GameInstance* gameInstance;

GameInstance* GetGameInstance() {
  return gameInstance;
}

int main(void)
{
  //BlockVertex::PackedColor c = { 255, 128, 70, 0 };
  //int a = *(int*)&c;
  //std::cout << (a & 255) << std::endl; // first
  //std::cout << (a >> 8 & 255) << std::endl;
  //std::cout << (a >> 16 & 255) << std::endl;
  //std::cout << (a >> 24 & 255) << std::endl;


  Engine::Start();
  gameInstance = new GameInstance();
  gameInstance->Init();
  Engine::Run(&_CubeGameInstanceTickCallback);
  return 0;
}