#include "Player.h"
#include "../Graphics/OpenGL/Camera/Camera.h"
#include "PlayerInputComponent.h"
#include "../World/World.h"

Player::Player()
{
	camera = new Camera();
	inputComponent = new PlayerInputComponent();

	inputComponent->SetPlayer(this);

	camera->Bind();
	inputComponent->Enable();
}

void Player::Tick(float DeltaTime)
{
	glm::dvec3 forward{ lookAt.x, lookAt.y, lookAt.z };

	GetWorld()->RaycastHit(location, location + forward * 1000.0);


	//std::cout << "player forward vector: " << forward.x << ", " << forward.y << ", " << forward.z << '\n';
}

void Player::TestInput(InputMods mods)
{
}

void Player::TestEmpty()
{
	cubeLog("empty called");
}
