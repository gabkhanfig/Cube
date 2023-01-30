#include "CubeInput.h"
#include <Engine.h>
#include <Window/Window.h>
#include "InputComponent.h"

darray<InputComponent*> CubeInput::activeInputComponents;
glm::dvec2 CubeInput::previousCursorPos = glm::dvec2(0, 0);

void CubeInput::ButtonInputCallback(GlobalString button, EInputAction action, InputMods mods)
{
	if (button == "Esc") {
		cubeLog("Escape key was pressed. Closing Cube.");
		engine->GetWindow()->Close();
	}

	for (ArrSizeT i = 0; i < activeInputComponents.Size(); i++) {
		if (action == EInputAction::Press) {
			activeInputComponents[i]->Press(button, mods);
		}
		else {
			activeInputComponents[i]->Release(button, mods);
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
