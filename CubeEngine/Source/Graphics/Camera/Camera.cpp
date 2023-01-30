#include "Camera.h"

Camera* Camera::activeCamera = nullptr;

glm::mat4 Camera::GetProjectionMatrix() const
{
	return glm::perspective(glm::radians(cameraFOV), 1.f / 1.f, 0.1f, float(chunkRenderDistance + 1) * 16.f * 16.f);
}

glm::mat4 Camera::GetViewMatrix() const
{
	constexpr glm::vec3 origin = { 0.f, 0.f, 0.f };
	return glm::lookAt(origin, origin + forward, GetUpVector());
}

Camera::Camera()
{
	degreeRotation = { 0, -80, -10 };
	forward = { 0, 0, 0 };
}

void Camera::Bind()
{
	activeCamera = this;
}

void Camera::Unbind()
{
	activeCamera = nullptr;
}

Camera* Camera::GetActiveCamera()
{
	return activeCamera;
}

bool Camera::IsBound() const
{
	return this == activeCamera;
}

glm::mat4 Camera::GetMvpMatrix() const
{
	return GetProjectionMatrix() * GetViewMatrix();
}

void Camera::SetForwardVector(glm::vec3 update)
{
	forward = update;
}
