#include "PlayerInput.h"
#include <Game/Player/Player.h>
#include <Engine/Input/UserInput.h>
#include <glm/gtx/rotate_vector.hpp>
#include <Engine/Render/Camera/Camera.h>

constexpr glm::vec3 upVector = glm::dvec3(0.0, 1.0, 0.0);



static Player* GetPlayer() 
{
	return Player::GetPlayer();
}

void PlayerInput::SetupPlayerInput(Player* player)
{
	Camera::GetActiveCamera()->SetFollowingEntity(player);
	UserInput::setHoldCallback(GLFW_KEY_W, PlayerInput::MovePlayerForwards);
	UserInput::setHoldCallback(GLFW_KEY_S, PlayerInput::MovePlayerBackwards);
	UserInput::setHoldCallback(GLFW_KEY_D, PlayerInput::MovePlayerRight);
	UserInput::setHoldCallback(GLFW_KEY_A, PlayerInput::MovePlayerLeft);
	UserInput::setHoldCallback(GLFW_KEY_SPACE, PlayerInput::MovePlayerUp);
	UserInput::setHoldCallback(GLFW_KEY_LEFT_CONTROL, PlayerInput::MovePlayerDown);
}

void PlayerInput::MovePlayerForwards(float deltaTime)
{
	GetPlayer()->ForwardHold(deltaTime, 1.f);
}

void PlayerInput::MovePlayerBackwards(float deltaTime)
{
	GetPlayer()->ForwardHold(deltaTime, -1.f);
}

void PlayerInput::MovePlayerRight(float deltaTime)
{
	GetPlayer()->RightHold(deltaTime, 1.f);
}

void PlayerInput::MovePlayerLeft(float deltaTime)
{
	GetPlayer()->RightHold(deltaTime, -1.f);
}

void PlayerInput::MovePlayerUp(float deltaTime)
{
	GetPlayer()->UpHold(deltaTime, 1.f);
}

void PlayerInput::MovePlayerDown(float deltaTime)
{
	GetPlayer()->UpHold(deltaTime, -1.f);
}
