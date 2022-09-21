#include "TickManager.h"
#include <Engine/Render/Renderer.h>
#include <GLFW/glfw3.h>
#include <Engine/Application/Application.h>
#include <Engine/Input/UserInput.h>
#include <Vendor/ImGui/imgui.h>
#include <Vendor/ImGui/imgui_impl_glfw.h>
#include <Vendor/ImGui/imgui_impl_opengl3.h>
#include <Game/State/GameInstance.h>

TickManager* TickManager::tickManager;

void TickManager::RunGameLoop()
{
	TickManager::tickManager = new TickManager();

	GLFWwindow* window = Application::GetWindow();

  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(Application::GetWindow(), true);
  ImGui_ImplOpenGL3_Init("#version 460");

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    TickManager::tickManager->Tick();
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void TickManager::Tick()
{
  previous = current;
  current = glfwGetTime();
  deltaTime = current - previous;

  Renderer::Clear();

  UserInput::Tick(deltaTime);

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  GameInstance::GetGameInstance()->Tick(deltaTime);
  //Renderer::Frame(deltaTime);

  TestImGuiFrame();

  /* Swap front and back buffers */
  glfwSwapBuffers(Application::GetWindow());

  /* Poll for and process events */
  glfwPollEvents();
}

void TickManager::TestImGuiFrame() 
{
  ImGui::Begin("Cube Application Debug Data");        

  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  ImGui::End();

  ImGui::Render();

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
