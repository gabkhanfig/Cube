#include "Player.h"
#include "../Graphics/OpenGL/Camera/Camera.h"
#include "PlayerInputComponent.h"
#include "../World/World.h"
#include "../World/Chunk/Chunk.h"
#include "../World/Block/BlockFactory.h"

Player::Player()
	: forwardInputHeld(false), backwardInputHeld(false), rightInputHeld(false), leftInputHeld(false), upInputHeld(false), downInputHeld(false)
{
	camera = new Camera();
	camera->SetForwardVectorReference(&rotation);

	inputComponent = new PlayerInputComponent();

	inputComponent->SetPlayer(this);
	SetupInputBinds();

	camera->Bind();
	inputComponent->Enable();
}

void Player::Tick(float deltaTime)
{
	RaycastHitResult hitResult = GetWorld()->RaycastHit(location, location + (GetRotation() * 1000.0));
	if (hitResult.success == RaycastHitResult::HitSuccess::Block) {
		highlightedObject = hitResult;
	}
	else {
		highlightedObject = RaycastHitResult();
	}
	//std::cout << "player forward vector: " << forward.x << ", " << forward.y << ", " << forward.z << '\n';
	UpdatePositionFromInputs(deltaTime);
}

void Player::SetupInputBinds()
{
	inputComponent->BindPressCallback("LMB", &Player::InputAttack);
	inputComponent->BindPressCallback("RMB", &Player::InputUse);

	inputComponent->BindPressCallback("W", &Player::InputPressForward);
	inputComponent->BindReleaseCallback("W", &Player::InputReleaseForward);
	inputComponent->BindPressCallback("S", &Player::InputPressBackward);
	inputComponent->BindReleaseCallback("S", &Player::InputReleaseBackward);
	inputComponent->BindPressCallback("D", &Player::InputPressRight);
	inputComponent->BindReleaseCallback("D", &Player::InputReleaseRight);
	inputComponent->BindPressCallback("A", &Player::InputPressLeft);
	inputComponent->BindReleaseCallback("A", &Player::InputReleaseLeft);
	inputComponent->BindPressCallback("Space", &Player::InputPressUp);
	inputComponent->BindReleaseCallback("Space", &Player::InputReleaseUp);
	inputComponent->BindPressCallback("Ctrl", &Player::InputPressDown);
	inputComponent->BindReleaseCallback("Ctrl", &Player::InputReleaseDown);
}

void Player::InputUse(InputMods mods)
{
	if (highlightedObject.success != RaycastHitResult::HitSuccess::Block) {
		cubeLog("InputUse not looking at a block");
		return;
	}
	Block b = BlockFactory::CreateBlock("stone");
	WorldPosition pos = { glm::dvec3(highlightedObject.position.x, highlightedObject.position.y + 1.0, highlightedObject.position.z) };
	GetWorld()->SetBlockAt(pos, b);
}

void Player::InputAttack(InputMods mods)
{
	if (highlightedObject.success != RaycastHitResult::HitSuccess::Block) {
		cubeLog("InputAttack not looking at a block");
		return;
	}
	WorldPosition pos = { glm::dvec3(highlightedObject.position.x, highlightedObject.position.y, highlightedObject.position.z) };
	GetWorld()->DestroyBlockAt(pos);
}

void Player::UpdatePositionFromInputs(float deltaTime)
{
	const double forwardScale = (forwardInputHeld ? 1 : 0) + (backwardInputHeld ? -1 : 0);
	if (forwardScale != 0) {
		AddForwardInput(forwardScale * double(deltaTime) * 20.0);
	}

	const double rightScale = (rightInputHeld ? 1 : 0) + (leftInputHeld ? -1 : 0);
	if (rightScale != 0) {
		AddRightInput(rightScale * double(deltaTime) * 20.0);
	}

	const double verticalScale = (upInputHeld ? 1 : 0) + (downInputHeld ? -1 : 0);
	if (verticalScale != 0) {
		AddVerticalInput(verticalScale * double(deltaTime) * 20.0);
	}
}
