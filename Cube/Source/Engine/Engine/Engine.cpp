#include "Engine.h"
#include <Engine/Render/Renderer.h>
#include <Engine/Tick/TickManager.h>
#include <Engine/Input/UserInput.h>

Engine* Engine::engine;

void Engine::StartEngine()
{
	engine = new Engine();

	UserInput::InitializeDefaultCallbacks();
	Renderer::LoadGraphicsAPI();
	TickManager::RunGameLoop();
}
