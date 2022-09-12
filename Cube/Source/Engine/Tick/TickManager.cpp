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

void TickManager::TestImGuiFrame() {
  /*ImGui::Begin("My name is a test window");
  ImGui::Text("hello world! .. lala");
  ImGui::End();
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());*/

  bool show_demo_window = true;
  bool show_another_window = false;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);



  static float f = 0.0f;
  static int counter = 0;

  ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

  //ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
  //ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
  //ImGui::Checkbox("Another Window", &show_another_window);

  //ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
  //ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

  //if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
  //  counter++;
  //ImGui::SameLine();
  //ImGui::Text("counter = %d", counter);

  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  ImGui::End();

  ImGui::Render();



  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
