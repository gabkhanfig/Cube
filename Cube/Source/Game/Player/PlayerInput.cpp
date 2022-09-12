#include "PlayerInput.h"
#include <Game/Player/Player.h>
#include <Engine/Input/UserInput.h>
#include <glm/gtx/rotate_vector.hpp>
#include <Engine/Render/Camera/Camera.h>

constexpr glm::dvec3 upVector = glm::dvec3(0.0, 1.0, 0.0);

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
	switch (GetPlayer()->movementMode) {
	case PlayerMovementMode::Free:
		GetPlayer()->position += deltaTime * GetPlayer()->movementSpeed * GetPlayer()->rotation;
		break;
	default:
		glm::dvec3 rotation = glm::rotate(GetPlayer()->rotation, double(glm::radians(90.f)), glm::dvec3(0.0f, 1.0f, 0.0f));
		GetPlayer()->position += deltaTime * GetPlayer()->movementSpeed * glm::normalize(glm::cross(rotation, upVector));
		break;
	}
}

void PlayerInput::MovePlayerBackwards(float deltaTime)
{
	switch (GetPlayer()->movementMode) {
	case PlayerMovementMode::Free:
		GetPlayer()->position -= deltaTime * GetPlayer()->movementSpeed * GetPlayer()->rotation;
		break;
	default:
		glm::dvec3 rotation = glm::rotate(GetPlayer()->rotation, double(glm::radians(90.f)), glm::dvec3(0.0f, 1.0f, 0.0f));
		GetPlayer()->position -= deltaTime * GetPlayer()->movementSpeed * glm::normalize(glm::cross(rotation, upVector));
		break;
	}
}

void PlayerInput::MovePlayerRight(float deltaTime)
{
	GetPlayer()->position += deltaTime * GetPlayer()->movementSpeed * glm::normalize(glm::cross(GetPlayer()->rotation, upVector));
}

void PlayerInput::MovePlayerLeft(float deltaTime)
{
	GetPlayer()->position -= deltaTime * GetPlayer()->movementSpeed * glm::normalize(glm::cross(GetPlayer()->rotation, upVector));
}

void PlayerInput::MovePlayerUp(float deltaTime)
{
	if (GetPlayer()->movementMode != PlayerMovementMode::Normal) {
		GetPlayer()->position.y += deltaTime * GetPlayer()->movementSpeed;
	}
}

void PlayerInput::MovePlayerDown(float deltaTime)
{
	if (GetPlayer()->movementMode != PlayerMovementMode::Normal) {
		GetPlayer()->position.y -= deltaTime * GetPlayer()->movementSpeed;
	}
}
