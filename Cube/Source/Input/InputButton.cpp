#include "InputButton.h"

InputButton::InputButton(GlobalString buttonName)
	: button(buttonName), pressFunc(nullptr), releaseFunc(nullptr), holdTime(0), isHeld(false)
{}

void InputButton::Press(InputMods mods)
{
	isHeld = true;
	holdTime = 0;
	cubeLog("pressed button");
	if (pressFunc) pressFunc(mods);
}

void InputButton::Release(InputMods mods)
{
	isHeld = false;
	cubeLog("released button");
	if (releaseFunc) releaseFunc(mods);
}

void InputButton::Tick(float deltaTime)
{
	if (!isHeld) return;

	holdTime += deltaTime;
}

void InputButton::SetPressCallback(ActionFunc callback)
{
	pressFunc = callback;
}

void InputButton::SetReleaseCallback(ActionFunc callback)
{
	releaseFunc = callback;
}

void InputButton::Reset()
{
	holdTime = 0;
	isHeld = false;
}
