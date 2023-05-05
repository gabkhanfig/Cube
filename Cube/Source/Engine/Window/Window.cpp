#include "Window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assert.h>
#include "../EngineCore.h"
#include <string>

bool isGlfwInitialized = false;

void Window::InitializeGLFW()
{
	if (!glfwInit())
		abort();

	isGlfwInitialized = true;
	//glfwWindowHint(GLFW_DEPTH_BITS, 24);
}

void Window::PollEvents()
{
	glfwPollEvents();
}

void Window::TerminateGLFW()
{
	glfwTerminate();
}

Window::Window(int inWidth, int inHeight, string inTitle, GLFWmonitor* inMonitor)
	: width(inWidth), height(inHeight), title(inTitle), monitor(inMonitor)
{
	checkm(isGlfwInitialized, "GLFW must be initialized before creating a window. Call Window::Initialize().");

	window = glfwCreateWindow(width, height, title.CStr(), monitor, nullptr);
	if (!window) {
		glfwTerminate();
	}
	/* Make the window's context current */
	//glfwMakeContextCurrent(window);
	//glfwSwapInterval(0);
}

bool Window::ShouldWindowClose() const
{
	return glfwWindowShouldClose(window);
}

void Window::Close()
{
	glfwSetWindowShouldClose(window, 1);
}

void Window::SetGLFWContextOnCallingThread(Window* window)
{
	//std::cout << std::this_thread::get_id() << std::endl;
	glfwMakeContextCurrent(window->GetGlfwWindow());
	glfwSwapInterval(0); // No FPS Limit
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(window);
}

void Window::PrintWindowInfo()
{
	int major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
	int minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
	int rev = glfwGetWindowAttrib(window, GLFW_CONTEXT_REVISION);
	std::cout << "GLFW Window Attribute OpenGL version : " << std::to_string(major) << '.' << std::to_string(minor) << '.' << std::to_string(rev) << '\n';
	std::cout << "Supported OpenGL is " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Supported GLSL is " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}
