#pragma once

#include <CubeCore.h>
#include "InputButton.h"

struct GLFWwindow;

class UserInput 
{
private: 

	static std::unordered_map<int, InputButton*> inputCallbacks;
	static glm::vec2 previousCursorPos;

public:

	static void InitializeDefaultCallbacks();

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	static void MouseCursorPositionCallback(GLFWwindow* window, double xpos, double ypos);

	/* Set a key press callback. */
	static void setPressCallback(int input, button_callback inputFuncPtr);

	/* Set a key release callback. */
	static void setReleaseCallback(int input, button_callback inputFuncPtr);

	/* Set a key hold callback. */
	static void setHoldCallback(int input, button_hold_callback inputFuncPtr);

	/* Map a key with optional function pointers for it's callbacks. */
	static InputButton* MapInput(int _input, button_callback _press = nullptr, button_callback _release = nullptr, button_hold_callback _hold = nullptr);

	/* Get a key object held within the key callbacks map. */
	static InputButton* GetInputObject(int _input);

	static void Tick(float deltaTime);
};