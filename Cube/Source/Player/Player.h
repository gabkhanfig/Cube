#pragma once

#include "../Entity/Entity.h"
#include "../Engine/Input/UserInput.h"
#include "../World/Raycast.h"
#include "../World/Block/Block.h"

class Camera;
class PlayerInputComponent;

class Player : public Entity 
{
private:

	Camera* camera;

	PlayerInputComponent* inputComponent;

	RaycastHitResult highlightedObject;

	uint8 forwardInputHeld : 1;
	uint8 backwardInputHeld : 1;
	uint8 rightInputHeld : 1;
	uint8 leftInputHeld : 1;
	uint8 upInputHeld : 1;
	uint8 downInputHeld : 1;

public:

	Player();

	virtual void Tick(float deltaTime) override;

	Camera* GetCamera() const { return camera; }

private:

#pragma region Inputs

	void SetupInputBinds();

	inline void InputPressForward(InputMods mods) { forwardInputHeld = true; }
	inline void InputReleaseForward(InputMods mods) { forwardInputHeld = false; }
	inline void InputPressBackward(InputMods mods) { backwardInputHeld = true; }
	inline void InputReleaseBackward(InputMods mods) { backwardInputHeld = false; }
	inline void InputPressRight(InputMods mods) { rightInputHeld = true; }
	inline void InputReleaseRight(InputMods mods) { rightInputHeld = false; }
	inline void InputPressLeft(InputMods mods) { leftInputHeld = true; }
	inline void InputReleaseLeft(InputMods mods) { leftInputHeld = false; }
	inline void InputPressUp(InputMods mods) { upInputHeld = true; }
	inline void InputReleaseUp(InputMods mods) { upInputHeld = false; }
	inline void InputPressDown(InputMods mods) { downInputHeld = true; }
	inline void InputReleaseDown(InputMods mods) { downInputHeld = false; }

	void InputUse(InputMods mods);
	void InputAttack(InputMods mods);

	void UpdatePositionFromInputs(float deltaTime);

#pragma endregion

};