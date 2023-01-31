#pragma once

#include "../Entity/Entity.h"
#include <Input/UserInput.h>

class Camera;
class PlayerInputComponent;

class Player : public Entity 
{
private:

	Camera* camera;

	PlayerInputComponent* inputComponent;

public:

	Player();

	virtual void Tick(float DeltaTime) override;

	Camera* GetCamera() const { return camera; }

	void TestInput(InputMods mods);

	void TestEmpty();



};