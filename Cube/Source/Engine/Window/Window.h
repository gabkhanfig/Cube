#pragma once

#include "../../Core/EngineTypes.h"

struct GLFWmonitor;
struct GLFWwindow;

/* GLFW Window wrapper class. */
class Window 
{
private:

	/* Title of the window. */
	String title;

	/* Set to null for windowed mode. */
	GLFWmonitor* monitor;

	/* GLFW window object. */
	GLFWwindow* window;

	/* Window width. */
	int width;

	/* Window height. */
	int height;

public:

	/* Initializes GLFW. */
	static void InitializeGLFW();

	/* Polls GLFW events such as keyboard and mouse input. */
	static void PollEvents();

	/**/
	static void TerminateGLFW();

	/* Constructs a window and sets it as the current context. Set inMonitor to nullptr to use windowed mode.
	@param inWidth: Window width.
	@param inHeight: Window height.
	@param inTitle: Window title.
	@param inMonitor: Monitor to use. Use nullptr for windowed mode. */
	Window(int inWidth, int inHeight, String inTitle, GLFWmonitor* inMonitor = nullptr);

	/* Swap the front and back buffers of this window. */
	void SwapBuffers();

	/**/
	void PrintWindowInfo();

	/* Get if this window should close. */
	bool ShouldWindowClose() const;

	void Close();

	/* Get the title of the window. */
	forceinline String GetTitle() const { return title; }

	/* Get the monitor this window is using. Can be nullptr for windowed mode. */
	forceinline GLFWmonitor* GetGlfwMonitor() const { return monitor; }

	/* Get the GLFW window object. */
	forceinline GLFWwindow* GetGlfwWindow() const { return window; }

	/* Get the width of the window. */
	forceinline int GetWidth() const { return width; }

	/* Get the height of the window. */
	forceinline int GetHeight() const { return height; }

	static void SetGLFWContextOnCallingThread(Window* window);



};