#include "PlayerInputComponent.h"
#include "../Input/CubeInput.h"
#include <Graphics/Camera/Camera.h>
#include "Player.h"
#include "../Input/InputButton.h"
#include <functional>

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
	player->SetLookAt(forward);

	// TODO make player automatically set the forward vector
	player->GetCamera()->SetForwardVector(forward);
}

void PlayerInputComponent::Press(GlobalString button, InputMods mods)
{
	cubeLog("player input component press");
	if (button == "W") {
		AddPlayerForwardInput(1);
	}
	else if (button == "S") {
		AddPlayerForwardInput(-1);
	}
}

void PlayerInputComponent::AddPlayerForwardInput(float scale)
{
	glm::dvec3 location = player->GetLocation();
	location += 0.5 * double(scale) * glm::dvec3(player->GetLookAt().x, player->GetLookAt().y, player->GetLookAt().z);
	player->SetLocation(location);
	player->GetCamera()->SetPosition({ location.x, location.y, location.z });
}

void PlayerInputComponent::AddPlayerRightInput(float scale)
{
	glm::vec3 location = player->GetLocation();

}
