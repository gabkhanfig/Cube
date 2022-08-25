#pragma once

constexpr int defaultWindowWidth = 960;
constexpr int defaultWindowHeight = 540;
constexpr const char* windowName = "Cube :)";

struct GLFWwindow;

class Application 
{
private:

	static Application* application;

	static GLFWwindow* window;

	int windowWidth;
	int windowHeight;

private:

	Application();

	int initializeWindow();

public:

	inline static Application* GetApplication() { return Application::application; }
	inline static GLFWwindow* GetWindow() { return Application::window; }

	static int Run(int argc, char** argv);

	static void glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height);
	static void glfwErrorCallback(int error, const char* description);
	static void glfwCloseApplication(int mods);
};