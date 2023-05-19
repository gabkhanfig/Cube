#include "Player.h"
#include "../Graphics/OpenGL/Camera/Camera.h"
#include "PlayerInputComponent.h"
#include "../World/World.h"
#include "../World/Chunk/Chunk.h"
#include "../World/Block/Block.h"

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

	RaycastHitResult hitResult = GetWorld()->RaycastHit(location, location + forward * 1000.0);
	if (hitResult.success == RaycastHitResult::HitSuccess::block) {
		highlightedObject = hitResult;
	}
	else {
		highlightedObject = RaycastHitResult();
	}

	//std::cout << "player forward vector: " << forward.x << ", " << forward.y << ", " << forward.z << '\n';
}

void Player::TestInput(InputMods mods)
{
}

void Player::TestPlaceBlock()
{
	if (highlightedObject.success != RaycastHitResult::HitSuccess::block) {
		return;
	}
	cubeLog("test plaace block");
	Block* b = BlockFactory::GetBlockClass("stoneBlock")->NewBlock();
	WorldPosition pos = { glm::dvec3(highlightedObject.position.x, highlightedObject.position.y + 1.0, highlightedObject.position.z) };
	GetWorld()->SetBlockAt(pos, b);
}
