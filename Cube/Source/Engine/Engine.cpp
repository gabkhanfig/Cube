#include "Engine.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Input/UserInput.h"
#include "Window/Window.h"
#include "Tick/TickEngine.h"
#include "../Core/Utils/CompileTimeFiles.h"
#include "OpenGL/OpenGLInstance.h"

void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}

constexpr int windowWidth = 960;
constexpr int windowHeight = 540;

EngineInitializationParams::EngineInitializationParams()
	: gameInstance(nullptr),
	threadPoolNum(gk::ThreadPool::SystemThreadCount() - 2),
	resolution(windowWidth, windowHeight)
{
	gk_assertm(threadPoolNum >= 2, "Thread pool must have at least 2 threads. threadPoolNum: " << threadPoolNum);
	gk_assertm(resolution.x > 0, "Window resolution width must be greater than 0. resolution width: " << resolution.x);
	gk_assertm(resolution.y > 0, "Window resolution height must be greater than 0. resolution height: " << resolution.y);
}

Engine* engine = nullptr;

float Engine::GetDeltaTime() const
{
	return tick->GetDeltaTime();
}

void Engine::SwapGlfwBuffers()
{
	window->SwapBuffers();
}

void Engine::WaitForRenderThread(int64 millisecondTimeout)
{
	auto startWait = std::chrono::high_resolution_clock::now();
#ifdef CUBE_DEVELOPMENT
	while (!renderThread->IsReady()) {
		std::chrono::steady_clock::time_point now = std::chrono::high_resolution_clock::now();
		auto duration = now - startWait;
		if (std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() > millisecondTimeout) {
			cubeLog("Timed out waiting for render thread. Waited " + String::FromInt(millisecondTimeout) + "ms");
			DebugBreak();
		}
	}
#else 
	while (!renderThread->IsReady());
#endif
}

Engine::Engine() :
	useRenderThread(true),
	tick(nullptr),
	threadPool(new gk::ThreadPool(gk::ThreadPool::SystemThreadCount() - 2))
{
	input = new UserInput();
#ifdef CUBE_DEVELOPMENT
	const char* windowTitle = "Cube [Development Build]";
#else
	const char* windowTitle = "Cube";
#endif
	window = new Window(windowWidth, windowHeight, windowTitle);
	glfwSetInputMode(window->GetGlfwWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	renderThread = new gk::Thread();
	openGLInstance = new OpenGLInstance(renderThread);
}

void Engine::Initialize()
{
	IObject::_InitializePendingDeleteArray();

	CompileTimeFiles::LoadAllFiles();

	Window::InitializeGLFW();
	engine = new Engine();
	engine->window->SetGLFWContext(engine->renderThread);
	engine->openGLInstance->InitializeOpenGL(engine->renderThread, glm::ivec2(windowWidth, windowHeight), glm::vec3(0.2, 0.55, 0.8));

	UserInput::SetCallbacks();
}

void Engine::Run(gk::Event<void, float>* tickCallback)
{
	engine->tick = new TickEngine(tickCallback);
	engine->tick->RunEngineLoop();
	Window::TerminateGLFW();
}


