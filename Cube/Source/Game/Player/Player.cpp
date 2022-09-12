#include "Player.h"
#include <Game/Player/PlayerInput.h>

Player* Player::player;

Player::Player()
{
	position = { 0.0, 0.0, 0.0 };
	rotation = { 0.0, 0.0, 0.0 };
	velocity = { 0.0, 0.0, 0.0 };

	tickEnabled = true;

	movementMode = PlayerMovementMode::Free;
	movementState = PlayerMovementState::Grounded;

	movementSpeed = 10;
}

Player* Player::StartPlayer()
{
	player = new Player();
	PlayerInput::SetupPlayerInput(player);
	return player;
}
