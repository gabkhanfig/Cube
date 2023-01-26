#include "Player.h"
#include <Graphics/Camera/Camera.h>

Player::Player()
{
	camera = new Camera();
	camera->Bind();
}

void Player::Tick(float DeltaTime)
{
	const glm::vec3 cameraForward = camera->GetForward();
	rotation = glm::dvec3(cameraForward.x, cameraForward.y, cameraForward.z);
}
