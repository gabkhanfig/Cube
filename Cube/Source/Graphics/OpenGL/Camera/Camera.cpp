#include "Camera.h"

Camera* Camera::activeCamera = nullptr;

glm::mat4 Camera::GetProjectionMatrix() const
{
	return glm::perspective(glm::radians(cameraFOV), 960.f / 540.f, 0.01f, float(chunkRenderDistance + 1) * 16.f * 16.f);
}

glm::mat4 Camera::GetViewMatrix() const
{
	constexpr glm::vec3 origin = { 0, 0, 0 };
	const glm::vec3 _forward = glm::vec3(forward->x, forward->y, forward->z);
	return glm::lookAt(origin, origin + _forward, GetUpVector());
}

Camera::Camera()
{}

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
