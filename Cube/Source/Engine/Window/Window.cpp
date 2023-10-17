#include "Window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assert.h>
#include "../Engine.h"
#include <string>
#include "../OpenGL/OpenGLInstance.h"

bool isGlfwInitialized = false;

void Window::InitializeGLFW()
{
	if (!glfwInit()) {
		cubeLog("failed to initialize GLFW"_str);
		abort();
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

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

Window::Window(int inWidth, int inHeight, String inTitle, GLFWmonitor* inMonitor)
	: width(inWidth), height(inHeight), title(inTitle), monitor(inMonitor)
{
	gk_assertm(isGlfwInitialized, "GLFW must be initialized before creating a window. Call Window::Initialize().");

	window = glfwCreateWindow(width, height, title.cstr(), monitor, nullptr);
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

void Window::SetGLFWContext(gk::Thread* renderThread)
{
	if (renderThread == nullptr) {
		SetGLFWContextOnThread(this->window);
		return;
	}

	renderThread->BindFunction(std::bind(SetGLFWContextOnThread, this->window));
	renderThread->Execute();
	while (!renderThread->IsReady());
}

void Window::SetGLFWContextOnThread(GLFWwindow* window)
{
	assertOnRenderThread();
	glfwMakeContextCurrent(window);
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
