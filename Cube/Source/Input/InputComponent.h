#pragma once

#include <EngineCore.h>
#include <Input/UserInput.h>

class InputButton;

class InputComponent
{
private:

	std::unordered_map<GlobalString, InputButton*> buttons;

	glm::dvec2 previousCursorPos;

public:

	InputComponent();

	~InputComponent();

	virtual void Press(GlobalString button, InputMods mods);

	virtual void Release(GlobalString button, InputMods mods);

	virtual void Tick(float deltaTime);

	virtual void Cursor(double xpos, double ypos);

	void Bind();

	void Unbind();

private:

	InputButton* GetInputButton(GlobalString button);



};

