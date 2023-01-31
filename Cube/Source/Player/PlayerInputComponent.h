#pragma once

#include "../Input/InputComponent.h"
#include <functional>

class Player;

class PlayerInputComponent : public InputComponent
{
private:

	Player* player;

	float sensitivity;

	float pitch;

	float yaw;

public:

	PlayerInputComponent();

	void SetPlayer(Player* newPlayer);

	virtual void Cursor(double xpos, double ypos) override;

	virtual void Press(GlobalString button, InputMods mods);

	void AddPlayerForwardInput(float scale);

	void AddPlayerRightInput(float scale);

};
