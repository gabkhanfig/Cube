#include "TickEngine.h"
#include "../Engine.h"
#include "../Window/Window.h"
#include <GLFW/glfw3.h>
#include <string>

TickEngine::TickEngine(gk::Event<void, float>&& tickCallback)
	: callback(std::move(tickCallback)), previous(0), current(0), deltaTime(0), fpsCounter(0), tickNum(0)
{}

void TickEngine::RunEngineLoop()
{
	while (!engine->GetWindow()->ShouldWindowClose()) {
		//std::cout << "tickNum: " << tickNum << '\n';
		tickNum++;
		previous = current;
		current = glfwGetTime();
		deltaTime = static_cast<float>(current - previous);
		UpdateFps();

		callback.invoke(std::min(deltaTime, MAX_DELTA_TIME));

		/* Swap front and back buffers */
		//engine->GetWindow()->SwapBuffers();

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
		String windowWithFps = engine->GetWindow()->GetTitle();
		windowWithFps += "   FPS: ";
		windowWithFps += String::FromInt(static_cast<int64>(CurrentFps()));
		glfwSetWindowTitle(engine->GetWindow()->GetGlfwWindow(), windowWithFps.CStr());
	}
//#endif
}
