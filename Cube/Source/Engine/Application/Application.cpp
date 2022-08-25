#include "Application.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <CubeCore.h>
#include <iostream>
#include <Engine/Engine/Engine.h>

Application* Application::application;
GLFWwindow* Application::window;

Application::Application()
{
	windowWidth = defaultWindowWidth;
	windowHeight = defaultWindowHeight;
}

int Application::initializeWindow()
{
  /* Initialize the library */
  if (!glfwInit())
    return -1;

  glfwSetErrorCallback(Application::glfwErrorCallback);

#if RENDERER == OPEN_GL

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#endif

  //glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(windowWidth, windowHeight, windowName, NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  glfwSetFramebufferSizeCallback(window, Application::glfwFramebufferSizeCallback);

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  /* Unlimited FPS */
  glfwSwapInterval(0);

  return 1;
}


int Application::Run(int argc, char** argv)
{
  application = new Application();

  if (application->initializeWindow() == -1) 
    return -1;

  Engine::StartEngine();

  glfwTerminate();
  return 0;
}

void Application::glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
  application->windowWidth = width;
  application->windowHeight = height;
}

void Application::glfwErrorCallback(int error, const char* description)
{
  Print(description);
}

void Application::glfwCloseApplication(int mods)
{
  glfwSetWindowShouldClose(window, true);
}
