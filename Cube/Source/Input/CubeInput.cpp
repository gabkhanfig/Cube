#include "CubeInput.h"
#include <Engine.h>
#include <Window/Window.h>
#include "InputComponent.h"
#include <Input/InputMapping.h>

static std::unordered_map<GlobalString, InputButtonState> MakeButtonStates() {
	std::unordered_map<GlobalString, InputButtonState> buttonStates;
	const darray<GlobalString> mappedInputs = InputMapping::GetMappedInputs();
	for (ArrSizeT i = 0; i < mappedInputs.Size(); i++) {
		buttonStates.insert({ mappedInputs[i], InputButtonState() });
	}
	return buttonStates;
}

std::unordered_map<GlobalString, InputButtonState> CubeInput::buttonStates = MakeButtonStates();
darray<InputComponent*> CubeInput::activeInputComponents;
glm::dvec2 CubeInput::previousCursorPos = glm::dvec2(0, 0);

void CubeInput::ButtonInputCallback(GlobalString button, EInputAction action, InputMods mods)
{
	if (button == "Esc") {
		cubeLog("Escape key was pressed. Closing Cube.");
		engine->GetWindow()->Close();
	}

	if (action != EInputAction::Repeat) {
		auto pair = buttonStates.find(button);
		if (pair != buttonStates.end()) {
			pair->second.isHeld = action == EInputAction::Press;
		}
	}
	
	for (ArrSizeT i = 0; i < activeInputComponents.Size(); i++) {
		if (action == EInputAction::Press) {
			activeInputComponents[i]->Press(button, mods);
		}
		else if (action == EInputAction::Release) {
			activeInputComponents[i]->Release(button, mods);
		}
		else {
			
		}
	}
}

void CubeInput::CursorPositionCallback(double xpos, double ypos)
{
	for (ArrSizeT i = 0; i < activeInputComponents.Size(); i++) {
		activeInputComponents[i]->Cursor(xpos, ypos);
	}
	previousCursorPos = glm::dvec2(xpos, ypos);
}

void CubeInput::SetupGameCallbacks()
{
	engine->GetUserInput()->SetButtonInputCallback(CubeInput::ButtonInputCallback);
	engine->GetUserInput()->SetCursorPositionCallback(CubeInput::CursorPositionCallback);
}

void CubeInput::Tick(float deltaTime)
{
	for (ArrSizeT i = 0; i < activeInputComponents.Size(); i++) {
		activeInputComponents[i]->Tick(deltaTime);
	}
	for (auto& pair : buttonStates) {
		if (pair.second.isHeld) {
			pair.second.holdTime += deltaTime;
		}
	}
}

void CubeInput::BindInputComponent(InputComponent* inputComponent)
{
	activeInputComponents.Add(inputComponent);
}

void CubeInput::UnbindInputComponent(InputComponent* inputComponent)
{
	activeInputComponents.RemoveFirst(inputComponent);
}

glm::dvec2 CubeInput::GetPreviousCursorPos()
{
	return previousCursorPos;
}

InputButtonState CubeInput::GetButtonState(GlobalString button)
{
	auto pair = buttonStates.find(button);
	if (pair == buttonStates.end()) {
		throw std::invalid_argument("Cannot get button state of a button that doesn't exist");
	}
	return pair->second;
}
