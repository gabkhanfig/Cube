#include "GameInstance.h"
#include <Game/World/World.h>
#include <Game/World/Dimension/Dimension.h>
#include <Game/World/Chunk/Chunk.h>
#include <Game/World/Block/Block.h>
#include <Game/World/Block/Factory/BlockFactory.h>
#include <Engine/Render/Renderer.h>
#include <Game/Player/Player.h>

GameInstance* GameInstance::gameInstance;

GameInstance::GameInstance()
{
}

GameInstance* GameInstance::StartGame()
{
	gameInstance = new GameInstance();

	BlockFactory::InjectAllBlocks();
	Block* stone = BlockFactory::GetBlock(1);
	Print(stone->GetBlockName());

	gameInstance->Init();
	return gameInstance;
}

void GameInstance::Init()
{
	Player::StartPlayer();

	World::Init();
}

void GameInstance::Tick(float deltaTime)
{
	Renderer::Frame(deltaTime);
	World::Tick(deltaTime);
}
