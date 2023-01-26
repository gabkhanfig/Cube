#include "UserInput.h"
#include "InputMapping.h"
#include <GLFW/glfw3.h>
#include "../Graphics/Camera/Camera.h"
#include "../Engine.h"
#include "../Window/Window.h"

UserInput::UserInput()
{
	previousCursorPos = { 0, 0 };
}

void UserInput::SetCallbacks()
{
	GLFWwindow* window = engine->GetWindow()->GetGlfwWindow();
	glfwSetKeyCallback(window, UserInput::KeyCallback);
	glfwSetMouseButtonCallback(window, UserInput::MouseButtonCallback);
	glfwSetCursorPosCallback(window, UserInput::MouseCursorPositionCallback);
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

void UserInput::ButtonInput(int key, int action, int mods)
{
	if (key == GLFW_KEY_SPACE) {
		std::cout << "spacebar :D !" << std::endl;
	}
}

void UserInput::CursorInput(double xpos, double ypos)
{
	const glm::dvec2 offset = glm::dvec2(xpos - previousCursorPos.x, previousCursorPos.y - ypos);
	previousCursorPos = glm::dvec2(xpos, ypos);

	Camera* cam = Camera::GetActiveCamera();
	if (cam == nullptr) return;
	cam->CursorChangePosition(offset);


}
