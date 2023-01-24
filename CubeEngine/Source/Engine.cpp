#include "Engine.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Input/UserInput.h"
#include "Window/Window.h"

constexpr int windowWidth = 800;
constexpr int windowHeight = 800;

Engine* engine;

Engine::Engine()
{
	input = new UserInput();
	window = new Window(windowWidth, windowHeight, "Cube");
}

void Engine::Run()
{
	Window::InitializeGLFW();
	engine = new Engine();
	UserInput::SetCallbacks();
	gladLoadGL(); 
	glViewport(0, 0, windowWidth, windowHeight);
}
