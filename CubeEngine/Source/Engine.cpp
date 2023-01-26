#include "Engine.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Input/UserInput.h"
#include "Window/Window.h"
#include "Tick/TickEngine.h"

constexpr int windowWidth = 800;
constexpr int windowHeight = 800;

Engine* engine;

float Engine::GetDeltaTime() const
{
	return tick->GetDeltaTime();
}

Engine::Engine()
{
	input = new UserInput();
#ifdef DEVELOPMENT
	const char* windowTitle = "Cube [Development Build]";
#else
	const char* windowTitle = "Cube";
#endif
	window = new Window(windowWidth, windowHeight, windowTitle);
	tick = nullptr;
	glfwSetInputMode(window->GetGlfwWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Engine::Start()
{
	Window::InitializeGLFW();
	engine = new Engine();
	UserInput::SetCallbacks();
	gladLoadGL(); 
	glViewport(0, 0, windowWidth, windowHeight);
	glClearColor(0.1, 0.1, 0.1, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Engine::Run(_TickCallback tickCallback)
{
	engine->tick = new TickEngine(tickCallback);
	engine->tick->RunEngineLoop();
	Window::TerminateGLFW();
}
