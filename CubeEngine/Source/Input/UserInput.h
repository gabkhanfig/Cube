#pragma once

#include "../EngineCore.h"

struct GLFWwindow;

class UserInput
{
private:

	glm::dvec2 previousCursorPos;

private:

	/**/
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	/**/
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	/**/
	static void MouseCursorPositionCallback(GLFWwindow* window, double xpos, double ypos);

public:

	UserInput();

	static void SetCallbacks();

	void ButtonInput(int key, int action, int mods);

	void CursorInput(double xpos, double ypos);

};

