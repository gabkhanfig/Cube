#pragma once

#include "../Entity/Entity.h"

class Camera;

class Player : public Entity 
{
private:

	class Camera* camera;

public:

	Player();

	virtual void Tick(float DeltaTime) override;

};