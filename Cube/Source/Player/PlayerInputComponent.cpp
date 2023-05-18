#include "PlayerInputComponent.h"
#include "../Input/CubeInput.h"
#include "../Graphics/OpenGL/Camera/Camera.h"
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
	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	else if (pitch < -89.0f) {
		pitch = -89.0f;
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
}

void PlayerInputComponent::Tick(float deltaTime)
{
	static GlobalString WKey = "W";
	static GlobalString AKey = "A";
	static GlobalString SKey = "S";
	static GlobalString DKey = "D";
	static GlobalString SpaceKey = "Space";
	static GlobalString CtrlKey = "Ctrl";

	const float WKeyScale = CubeInput::GetButtonState(WKey).isHeld ? 1 : 0;
	const float SKeyScale = CubeInput::GetButtonState(SKey).isHeld ? -1 : 0;
	const float ForwardScale = WKeyScale + SKeyScale;
	if (ForwardScale != 0) {
		AddPlayerForwardInput(ForwardScale * deltaTime * 20);
	}

	const float AKeyScale = CubeInput::GetButtonState(AKey).isHeld ? -1 : 0;
	const float DKeyScale = CubeInput::GetButtonState(DKey).isHeld ? 1 : 0;
	const float RightScale = AKeyScale + DKeyScale;
	if (RightScale != 0) {
		AddPlayerRightInput(RightScale * deltaTime * 20);
	}

	const float SpaceKeyScale = CubeInput::GetButtonState(SpaceKey).isHeld ? 1 : 0;
	const float CtrlKeyScale = CubeInput::GetButtonState(CtrlKey).isHeld ? -1 : 0;
	const float VerticalScale = SpaceKeyScale + CtrlKeyScale;
	if (VerticalScale != 0) {
		AddPlayerVerticalInput(VerticalScale * deltaTime * 20);
	}
}

void PlayerInputComponent::AddPlayerForwardInput(float scale)
{
	glm::dvec3 location = player->GetLocation();
	location += double(scale) * player->GetForwardVector();
	player->SetLocation(location);
	player->GetCamera()->SetPosition({ location.x, location.y, location.z });
}

void PlayerInputComponent::AddPlayerRightInput(float scale)
{
	glm::dvec3 location = player->GetLocation();
	location += double(scale) * player->GetRightVector();
	player->SetLocation(location);
	player->GetCamera()->SetPosition({ location.x, location.y, location.z });
}

void PlayerInputComponent::AddPlayerVerticalInput(float scale)
{
	glm::dvec3 location = player->GetLocation();
	location.y += scale;
	player->SetLocation(location);
	player->GetCamera()->SetPosition({ location.x, location.y, location.z });
}
