#include "Camera.h"
#include <Engine/Input/UserInput.h>
#include <iostream>
#include <glm/gtx/rotate_vector.hpp>
#include <Game/Player/Player.h>
//#include <glm/gtx/vector_angle.hpp>

Camera* Camera::activeCamera;

Camera::Camera()
{
	sensitivity = cameraSensitivity;
	fov = cameraFOV;

	proj = glm::perspective(glm::radians(fov), 16.f / 9.f, 0.1f, 1000.f);

	pitch = 0.f;
	yaw = 0.f;
}

void Camera::SetActiveCamera(Camera* newActiveCamera)
{
	Camera::activeCamera = newActiveCamera;
}

void Camera::SetRotation(const glm::vec3& newRotation)
{
	if (followEntity) {
		followEntity->rotation = newRotation;
	}
}

void Camera::SetFollowingEntity(Entity* entity)
{
	followEntity = entity;
}

glm::mat4 Camera::GetMVPMatrix()
{
	const glm::vec3 position = followEntity->position;
	const glm::vec3 rotation = followEntity->rotation;

	glm::mat4 view;
	view = glm::lookAt(position, position + rotation, GetUpVector());

	return proj * view;
}
