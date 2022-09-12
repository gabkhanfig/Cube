#pragma once

#include <Game/Entity/Entity.h>
#include <Game/Player/PlayerMovementMode.h>

class Camera;
class PlayerInput;

class Player : public Entity 
{
	friend class PlayerInput;
	friend class Camera;

private:

	static Player* player;

	PlayerMovementMode movementMode;
	PlayerMovementState movementState;
	double movementSpeed;

public:

	Player();

	inline static Player* GetPlayer() { return player; }

	static Player* StartPlayer();
};