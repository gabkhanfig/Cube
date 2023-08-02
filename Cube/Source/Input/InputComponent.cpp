#include "InputComponent.h"
#include "../Engine/Input/UserInput.h"
#include "InputButton.h"
#include "CubeInput.h"

InputComponent::InputComponent()
{
	//static const darray<GlobalString> cubeUsedInputs = InputMapping::GetMappedInputs();
	//for (uint32 i = 0; i < cubeUsedInputs.Size(); i++) {
	//	GlobalString str = cubeUsedInputs[i];
	//	buttons.insert({ str, new InputButton(str) });
	//}
}

InputComponent::~InputComponent()
{
	Disable();
}

void InputComponent::Press(GlobalString button, InputMods mods)
{
	//InputButton* inputButton = GetInputButton(button);
	//gk_assertm(inputButton != nullptr, "Button on press was nullptr in input component");
	//inputButton->Press(mods);
}

void InputComponent::Release(GlobalString button, InputMods mods)
{
	//InputButton* inputButton = GetInputButton(button);
	//gk_assertm(inputButton != nullptr, "Button on release was nullptr in input component");
	//inputButton->Release(mods);
}

void InputComponent::Tick(float deltaTime)
{
	//for (auto& button : buttons) {
	//	button.second->Tick(deltaTime);
	//}
}

void InputComponent::Cursor(double xpos, double ypos)
{
}

void InputComponent::Enable()
{
	CubeInput::BindInputComponent(this);
}

void InputComponent::Disable()
{
	CubeInput::UnbindInputComponent(this);
}

InputButton* InputComponent::GetInputButton(GlobalString button)
{
	auto found = buttons.find(button);
	if (found != buttons.end()) {
		return found->second;
	}
	return nullptr;
}
