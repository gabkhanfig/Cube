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
	const glm::dvec2 offset = glm::dvec2(xpos - previous.x, previous.y - ypos) * sensitivity;

	yaw += offset.x;
	pitch += offset.y;
	if (pitch > 89.0) {
		pitch = 89.0;
	}
	else if (pitch < -89.0) {
		pitch = -89.0;
	}
	glm::dvec3 forward = {
		cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
		sin(glm::radians(pitch)),
		sin(glm::radians(yaw)) * cos(glm::radians(pitch))
	};
	player->SetRotation(forward);
}

void PlayerInputComponent::Press(GlobalString button, InputMods mods)
{
	auto found = pressCallbacks.find(button);
	if (found != pressCallbacks.end()) {
		for (ArrSizeT i = 0; i < found->second->Size(); i++) {
			PlayerInputCallback callback = found->second->At(i);
			(player->*callback)(mods);
		}
	}
}

void PlayerInputComponent::Release(GlobalString button, InputMods mods)
{
	auto found = releaseCallbacks.find(button);
	if (found != releaseCallbacks.end()) {
		for (ArrSizeT i = 0; i < found->second->Size(); i++) {
			PlayerInputCallback callback = found->second->At(i);
			(player->*callback)(mods);
		}
	}
}

void PlayerInputComponent::Tick(float deltaTime)
{}

void PlayerInputComponent::BindPressCallback(GlobalString button, PlayerInputCallback callback)
{
	auto found = pressCallbacks.find(button);
	if (found != pressCallbacks.end()) {
		checkm(!found->second->Contains(callback), "Binding the same player input press callback twice is not allowed");
		found->second->Add(callback);
	}
	else {
		darray<PlayerInputCallback>* callbacks = new darray<PlayerInputCallback>();
		callbacks->Add(callback);
		pressCallbacks.insert({ button, callbacks });
	}
}

void PlayerInputComponent::BindReleaseCallback(GlobalString button, PlayerInputCallback callback)
{
	auto found = releaseCallbacks.find(button);
	if (found != releaseCallbacks.end()) {
		checkm(!found->second->Contains(callback), "Binding the same player input release callback twice is not allowed");
		found->second->Add(callback);
	}
	else {
		darray<PlayerInputCallback>* callbacks = new darray<PlayerInputCallback>();
		callbacks->Add(callback);
		releaseCallbacks.insert({ button, callbacks });
	}
}
