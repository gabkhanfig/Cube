#include "Player.h"
#include "../Graphics/OpenGL/Camera/Camera.h"
#include "PlayerInputComponent.h"

Player::Player()
{
	camera = new Camera();
	inputComponent = new PlayerInputComponent();

	inputComponent->SetPlayer(this);

	camera->Bind();
	inputComponent->Enable();

	//std::function<void(Player*, InputMods)> f = std::bind(&Player::TestEmpty, this, std::placeholders::_1);
	//inputComponent->BindPlayerAction("Space", EInputAction::Press, );
}

void Player::Tick(float DeltaTime)
{
}

void Player::TestInput(InputMods mods)
{
}

void Player::TestEmpty()
{
	cubeLog("empty called");
}
