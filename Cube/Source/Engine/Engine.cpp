#include "Engine.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Input/UserInput.h"
#include "Window/Window.h"
#include "Tick/TickEngine.h"

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

Engine* engine;

float Engine::GetDeltaTime() const
{
	return tick->GetDeltaTime();
}

void Engine::SwapGlfwBuffers()
{
	window->SwapBuffers();
}

Engine::Engine() :
	useRenderThread(true),
	tick(nullptr)
{
	input = new UserInput();
#ifdef DEVELOPMENT
	const char* windowTitle = "Cube [Development Build]";
#else
	const char* windowTitle = "Cube";
#endif
	window = new Window(windowWidth, windowHeight, windowTitle);
	glfwSetInputMode(window->GetGlfwWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!useRenderThread) {
		renderThread = nullptr;
		Window::SetGLFWContextOnCallingThread(window);
	}
	else {
		renderThread = new gk::Thread();
		renderThread->BindFunction(std::bind(Window::SetGLFWContextOnCallingThread, window)); // Bind function for setting OpenGL context on render thread.
		//std::cout << std::this_thread::get_id() << std::endl;
		renderThread->Execute(); // Set OpenGL context on render thread.
		while (!renderThread->IsReady()); // Wait until render thread finishes execution.
	}
}

void Engine::InitializeOpenGL(Window* _window, glm::vec4 clearColor)
{
	gladLoadGL();
	glViewport(0, 0, _window->GetWidth(), _window->GetHeight());
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glEnable(GL_DEPTH_TEST | GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW); // See chunk mesh index buffer object. this works and i dont know why
}

void Engine::Start()
{
	Window::InitializeGLFW();
	engine = new Engine();
	UserInput::SetCallbacks();

	const glm::vec4 clearColor = glm::vec4(0.1, 0.1, 0.1, 1.0);
	if (!engine->useRenderThread) {
		InitializeOpenGL(engine->window, clearColor);
	}
	else {
		engine->renderThread->BindFunction(std::bind(Engine::InitializeOpenGL, engine->window, clearColor));
		engine->renderThread->Execute();
		while (!engine->renderThread->IsReady());
	}
}

void Engine::Run(_TickCallback tickCallback)
{
	engine->tick = new TickEngine(tickCallback);
	engine->tick->RunEngineLoop();
	Window::TerminateGLFW();
}
