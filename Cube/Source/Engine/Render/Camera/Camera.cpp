#include "Camera.h"
#include <Engine/Input/UserInput.h>
#include <iostream>
#include <glm/gtx/rotate_vector.hpp>
#include <Game/Player/Player.h>

Camera* Camera::activeCamera;

Camera::Camera()
{
	sensitivity = cameraSensitivity;
	fov = cameraFOV;

	//proj = glm::perspective(glm::radians(fov), 16.f / 9.f, 0.1f, 1000.f);

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

glm::mat4 Camera::GetMVPMatrix() const
{
	return GetProjectionMatrix() * GetViewMatrix();
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	return glm::perspective(glm::radians(fov), 16.f / 9.f, 0.1f, float(chunkRenderDistance + 1) * 16.f * 16.f);
}

glm::mat4 Camera::GetViewMatrix(const glm::dvec3& pos) const
{
	constexpr glm::vec3 origin = { 0.f, 0.f, 0.f };

	const glm::vec3 rotation = followEntity->GetRotation();
	return glm::lookAt(origin, origin + rotation, GetUpVector());
}
