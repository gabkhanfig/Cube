#pragma once

#include "../Input/InputComponent.h"

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

};
