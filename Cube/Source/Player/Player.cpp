#include "Player.h"
#include <Graphics/Camera/Camera.h>
#include "PlayerInputComponent.h"

Player::Player()
{
	camera = new Camera();
	inputComponent = new PlayerInputComponent();

	inputComponent->SetPlayer(this);

	camera->Bind();
	inputComponent->Bind();
}

void Player::Tick(float DeltaTime)
{
}
