#include "UserInput.h"
#include "InputMapping.h"
#include <GLFW/glfw3.h>
#include "../Graphics/Camera/Camera.h"
#include "../Engine.h"
#include "../Window/Window.h"

UserInput::UserInput()
{
	previousCursorPos = { 0, 0 };
	buttonInputCallback = nullptr;
	cursorCallback = nullptr;
}

void UserInput::SetCallbacks()
{
	GLFWwindow* window = engine->GetWindow()->GetGlfwWindow();
	glfwSetKeyCallback(window, UserInput::KeyCallback);
	glfwSetMouseButtonCallback(window, UserInput::MouseButtonCallback);
	glfwSetCursorPosCallback(window, UserInput::MouseCursorPositionCallback);
}

EInputAction UserInput::ActionToInputAction(int action)
{
	if (action == GLFW_PRESS) {
		return EInputAction::Press;
	}
	else {
		return EInputAction::Release;
	}
}

void UserInput::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	engine->GetUserInput()->ButtonInput(key, action, mods);
}

void UserInput::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	engine->GetUserInput()->ButtonInput(button, action, mods);
}

void UserInput::MouseCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	engine->GetUserInput()->CursorInput(xpos, ypos);
}

void UserInput::ButtonInput(int button, int action, int mods)
{
	if (buttonInputCallback) {
		GlobalString _button = InputMapping::GetInputString(button);
		if (_button == "") return;
		EInputAction _action = ActionToInputAction(action);
		InputMods _mods{ mods };
		buttonInputCallback(_button, _action, _mods);
	}
}

void UserInput::CursorInput(double xpos, double ypos)
{
	if (cursorCallback) {
		cursorCallback(xpos, ypos);
	}
}

void UserInput::SetButtonInputCallback(ButtonInputCallback callback)
{
	buttonInputCallback = callback;
}

void UserInput::SetCursorPositionCallback(CursorCallback callback)
{
	cursorCallback = callback;
}
