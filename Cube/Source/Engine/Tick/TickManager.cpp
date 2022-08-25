#include "TickManager.h"
#include <Engine/Render/Renderer.h>
#include <GLFW/glfw3.h>
#include <Engine/Application/Application.h>
#include <Engine/Input/UserInput.h>

TickManager* TickManager::tickManager;

void TickManager::RunGameLoop()
{
	TickManager::tickManager = new TickManager();

	GLFWwindow* window = Application::GetWindow();

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    TickManager::tickManager->Tick();
  }
}

void TickManager::Tick()
{
  previous = current;
  current = glfwGetTime();
  deltaTime = current - previous;

  Renderer::Clear();

  UserInput::Tick(deltaTime);

  Renderer::Frame(deltaTime);

  /* Swap front and back buffers */
  glfwSwapBuffers(Application::GetWindow());

  /* Poll for and process events */
  glfwPollEvents();
}
