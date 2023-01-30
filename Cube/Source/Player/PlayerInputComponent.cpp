#include "PlayerInputComponent.h"
#include "../Input/CubeInput.h"
#include <Graphics/Camera/Camera.h>
#include "Player.h"

PlayerInputComponent::PlayerInputComponent()
	: player(nullptr), sensitivity(0.2), pitch(0), yaw(0)
{}

void PlayerInputComponent::SetPlayer(Player* newPlayer)
{
	player = newPlayer;
}

void PlayerInputComponent::Cursor(double xpos, double ypos)
{
	const glm::dvec2 previous = CubeInput::GetPreviousCursorPos(); 
	const glm::vec2 offset = glm::vec2(xpos - previous.x, previous.y - ypos) * sensitivity;

	yaw += offset.x;
	pitch += offset.y;
	if (pitch > 89.9f) {
		pitch = 89.9f;
	}
	else if (pitch < -89.9f) {
		pitch = -89.9f;
	}
	glm::vec3 forward = {
		cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
		sin(glm::radians(pitch)),
		sin(glm::radians(yaw)) * cos(glm::radians(pitch))
	};
	//player->SetRotation(forward);
	player->GetCamera()->SetForwardVector(forward);
}
