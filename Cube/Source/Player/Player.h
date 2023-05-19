#pragma once

#include "../Entity/Entity.h"
#include "../Engine/Input/UserInput.h"
#include "../World/Raycast.h"

class Camera;
class PlayerInputComponent;
class Block;

class Player : public Entity 
{
private:

	Camera* camera;

	PlayerInputComponent* inputComponent;

	RaycastHitResult highlightedObject;

public:

	Player();

	virtual void Tick(float DeltaTime) override;

	Camera* GetCamera() const { return camera; }

	void TestInput(InputMods mods);

	void TestPlaceBlock();

};