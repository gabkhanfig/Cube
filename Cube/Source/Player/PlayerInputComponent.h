#pragma once

#include "../Input/InputComponent.h"
#include <functional>

class Player;

typedef void (Player::* PlayerInputCallback)(InputMods);

class PlayerInputComponent : public InputComponent
{
private:

	Player* player;

	double sensitivity;

	double pitch;

	double yaw;

	HashMap<GlobalString, darray<PlayerInputCallback>*> pressCallbacks;

	HashMap<GlobalString, darray<PlayerInputCallback>*> releaseCallbacks;

public:

	PlayerInputComponent();

	void SetPlayer(Player* newPlayer);

	virtual void Cursor(double xpos, double ypos) override;

	virtual void Press(GlobalString button, InputMods mods) override;

	virtual void Release(GlobalString button, InputMods mods) override;

	virtual void Tick(float deltaTime) override;

	void BindPressCallback(GlobalString button, PlayerInputCallback callback);

	void BindReleaseCallback(GlobalString button, PlayerInputCallback callback);
};
