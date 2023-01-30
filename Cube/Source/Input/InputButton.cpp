#include "InputButton.h"

InputButton::InputButton(GlobalString buttonName)
	: button(buttonName), holdTime(0), isHeld(false)
{}

void InputButton::Press(InputMods mods)
{
	isHeld = true;
	holdTime = 0;
	cubeLog("pressed button");
}

void InputButton::Release(InputMods mods)
{
	isHeld = false;
	cubeLog("released button");
}

void InputButton::Tick(float deltaTime)
{
	if (!isHeld) return;

	holdTime += deltaTime;
}

void InputButton::Reset()
{
	holdTime = 0;
	isHeld = false;
}
