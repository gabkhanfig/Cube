#include "UserInput.h"
#include <GLFW/glfw3.h>
#include <Engine/Application/Application.h>
#include <Engine/Render/Camera/Camera.h>

std::unordered_map<int, InputButton*> UserInput::inputCallbacks;
glm::vec2 UserInput::previousCursorPos;

void UserInput::InitializeDefaultCallbacks()
{
	glfwSetInputMode(Application::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetKeyCallback(Application::GetWindow(), UserInput::KeyCallback);
	glfwSetMouseButtonCallback(Application::GetWindow(), UserInput::MouseButtonCallback);
	glfwSetCursorPosCallback(Application::GetWindow(), UserInput::MouseCursorPositionCallback);

	MapInput(GLFW_KEY_ESCAPE, Application::glfwCloseApplication);
	MapInput(GLFW_KEY_W);
	MapInput(GLFW_KEY_A);
	MapInput(GLFW_KEY_S);
	MapInput(GLFW_KEY_D);
	MapInput(GLFW_KEY_E);
	MapInput(GLFW_KEY_Q);
	MapInput(GLFW_KEY_SPACE);
	MapInput(GLFW_KEY_LEFT_CONTROL);

	MapInput(GLFW_MOUSE_BUTTON_LEFT);
	MapInput(GLFW_MOUSE_BUTTON_RIGHT);
}

void UserInput::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	InputButton* input = GetInputObject(key);
	if (!input) return;
	input->Action(action, mods);
}

void UserInput::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	InputButton* input = GetInputObject(button);
	if (!input) return;
	input->Action(action, mods);
}

void UserInput::MouseCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	//Print("Mouse cursor position callback");

	float xoffset = xpos - previousCursorPos.x;
	float yoffset = previousCursorPos.y - ypos;
	previousCursorPos.x = xpos;
	previousCursorPos.y = ypos;

	Camera* cam = Camera::activeCamera;
	if (cam == nullptr) return;

	const float sensitivity = cam->sensitivity;
	xoffset *= sensitivity;
	yoffset *= sensitivity;


	cam->yaw += xoffset;
	cam->pitch += yoffset;

	if (cam->pitch > 89.0f) {
		cam->pitch = 89.0f;
	}
	if (cam->pitch < -89.0f) {
		cam->pitch = -89.0f;
	}

	glm::vec3 front;
	front.x = cos(glm::radians(cam->yaw)) * cos(glm::radians(cam->pitch));
	front.y = sin(glm::radians(cam->pitch));
	front.z = sin(glm::radians(cam->yaw)) * cos(glm::radians(cam->pitch));
	cam->rotation = glm::normalize(front);
}

void UserInput::setPressCallback(int input, button_callback inputFuncPtr)
{
	InputButton* inputObj = GetInputObject(input);
	if (!inputObj) return;
	inputObj->SetPressCallback(inputFuncPtr);
}

void UserInput::setReleaseCallback(int input, button_callback inputFuncPtr)
{
	InputButton* inputObj = GetInputObject(input);
	if (!inputObj) return;
	inputObj->SetReleaseCallback(inputFuncPtr);
}

void UserInput::setHoldCallback(int input, button_hold_callback inputFuncPtr)
{
	InputButton* inputObj = GetInputObject(input);
	if (!inputObj) return;
	inputObj->SetHoldCallback(inputFuncPtr);
}

InputButton* UserInput::MapInput(int _input, button_callback _press, button_callback _release, button_hold_callback _hold)
{
	InputButton* input = new InputButton(_input, _press, _release, _hold);
	inputCallbacks.insert({ _input, input });
	return input;
}

InputButton* UserInput::GetInputObject(int _input)
{
	auto search = inputCallbacks.find(_input);
	if (search != inputCallbacks.end()) {
		return search->second;
	}
	else {
		//std::cout << "didn't find " << _button << " for setting key press callback" << std::endl;
	}
	return nullptr;
}

void UserInput::Tick(float deltaTime)
{
	for (auto input : inputCallbacks) {
		input.second->Tick(deltaTime);
	}
}

