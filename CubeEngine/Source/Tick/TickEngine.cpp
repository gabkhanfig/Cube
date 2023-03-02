#include "TickEngine.h"
#include "../Engine.h"
#include "../Window/Window.h"
#include <GLFW/glfw3.h>
#include <string>

TickEngine::TickEngine(_TickCallback tickCallback)
	: callback(tickCallback), previous(0), current(0), deltaTime(0), fpsCounter(0)
{}

void TickEngine::RunEngineLoop()
{
	while (!engine->GetWindow()->ShouldWindowClose()) {
		previous = current;
		current = glfwGetTime();
		deltaTime = current - previous;
		UpdateFps();

		callback(deltaTime);

		/* Swap front and back buffers */
		engine->GetWindow()->SwapBuffers();

		/* Poll for and process events */
		Window::PollEvents();
	}
}

void TickEngine::UpdateFps()
{
//#ifdef DEVELOPMENT
	const float fpsCounterUpdateInterval = 0.5;

	fpsCounter += deltaTime;
	if (fpsCounter >= fpsCounterUpdateInterval) {
		fpsCounter = 0; 
		std::string windowWithFps;
		windowWithFps += engine->GetWindow()->GetTitle().CStr();
		windowWithFps += "   FPS: ";
		windowWithFps += std::to_string((int)CurrentFps());
		glfwSetWindowTitle(engine->GetWindow()->GetGlfwWindow(), windowWithFps.c_str());
	}
//#endif
}
