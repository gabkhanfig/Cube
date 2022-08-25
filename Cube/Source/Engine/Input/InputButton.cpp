#include "InputButton.h"
#include <CubeCore.h>

InputButton::InputButton(int _button, button_callback _press, button_callback _release, button_hold_callback _hold)
{
	button = _button;
	press = _press;
	release = _release;
	hold = _hold;
	isPressed = false;
}

void InputButton::Tick(float deltaTime)
{
	HoldKey(deltaTime);
}

void InputButton::Action(int _action, int mods)
{
	if (_action == GLFW_PRESS) {
		PressKey(mods);
	}
	else if (_action == GLFW_RELEASE) {
		ReleaseKey(mods);
	}
}

void InputButton::PressKey(int mods)
{
	isPressed = true;
	if (press == nullptr) {
		//print("null press callback");
		return;
	}
	press(mods);
}

void InputButton::ReleaseKey(int mods)
{
	isPressed = false;
	if (release == nullptr) {
		//print("null release callback");
		return;
	}
	release(mods);
}

void InputButton::HoldKey(float deltaTime)
{
	if (isPressed) {
		if (hold == nullptr) {
			//print("null hold callback");
			return;
		}
		hold(deltaTime);
	}
}
