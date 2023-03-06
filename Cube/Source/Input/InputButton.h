#pragma once

#include "../Engine/EngineCore.h"
#include "../Engine/Input/UserInput.h"
#include <functional>

struct InputButtonState
{
	float holdTime;
	bool isHeld;

	InputButtonState() : holdTime(0), isHeld(false) {}
};



class InputButton
{
public:

	typedef std::function<void(InputMods)> ActionFunc;

private:

	const GlobalString button;

	ActionFunc pressFunc;

	ActionFunc releaseFunc;

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

	void SetPressCallback(ActionFunc callback);

	void SetReleaseCallback(ActionFunc callback);

	/* Called by owning input component when it unbinds. */
	void Reset();

	/**/
	float GetHoldTime() const { return holdTime; }

	/**/
	bool IsHeld() const { return isHeld; }
};
