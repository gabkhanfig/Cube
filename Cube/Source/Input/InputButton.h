#pragma once

#include <EngineCore.h>
#include <Input/UserInput.h>

class InputButton
{
public:



private:

	const GlobalString button;

	float holdTime;

	bool isHeld;

public:

	/**/
	InputButton(GlobalString buttonName);

	/**/
	void Press(InputMods mods);

	/**/
	void Release(InputMods mods);

	/**/
	void Tick(float deltaTime);

	/* Called by owning input component when it unbinds. */
	void Reset();

	/**/
	float GetHoldTime() const { return holdTime; }

	/**/
	bool IsHeld() const { return isHeld; }
};
