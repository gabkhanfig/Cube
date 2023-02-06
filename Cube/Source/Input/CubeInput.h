#pragma once

#include <Input/UserInput.h>
#include "InputButton.h"

class InputComponent;

class CubeInput
{
private:

	static std::unordered_map<GlobalString, InputButtonState> buttonStates;

	static darray<GlobalString> cubeUsedInputs;

	static darray<InputComponent*> activeInputComponents;
	
	static glm::dvec2 previousCursorPos;

public:

	/* Handles game relevant input. Used as a callback from the engine. */
	static void ButtonInputCallback(GlobalString button, EInputAction action, InputMods mods);

	/**/
	static void CursorPositionCallback(double xpos, double ypos);

	static void SetupGameCallbacks();

	static void Tick(float deltaTime);

	static void BindInputComponent(InputComponent* inputComponent);

	static void UnbindInputComponent(InputComponent* inputComponent);

	static glm::dvec2 GetPreviousCursorPos();

	static InputButtonState GetButtonState(GlobalString button);

};