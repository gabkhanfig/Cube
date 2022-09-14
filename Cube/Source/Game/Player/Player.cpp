#include "Player.h"
#include <Game/Player/PlayerInput.h>

Player* Player::player;

Player::Player()
{
	position = { -1.0, 8.0, -1.0 };
	rotation = { 0.0, 0.0, 0.0 };
	velocity = { 0.0, 0.0, 0.0 };

	tickEnabled = true;

	movementMode = PlayerMovementMode::Flying;
	movementState = PlayerMovementState::Grounded;

	movementSpeed = 20;
}

Player* Player::StartPlayer()
{
	player = new Player();
	PlayerInput::SetupPlayerInput(player);
	return player;
}

void Player::Tick(float deltaTime)
{
	Entity::Tick(deltaTime);
}
