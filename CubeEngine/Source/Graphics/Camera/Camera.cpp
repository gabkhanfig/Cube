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

void Camera::CursorChangePosition(const glm::dvec2 offset)
{
	const glm::dvec2 offSens = offset * cameraSensitivity;
	degreeRotation += glm::vec3(offSens.x, offSens.y, 1);

	if (degreeRotation.y > 89.9) {
		degreeRotation.y = 89.9f;
	}
	else if (degreeRotation.y < -89.9) {
		degreeRotation.y = -89.9f;
	}

	forward = glm::normalize(glm::vec3
	{
		cos(glm::radians(degreeRotation.x)) * cos(glm::radians(degreeRotation.y)),
		sin(glm::radians(degreeRotation.y)),
		sin(glm::radians(degreeRotation.x)) * cos(glm::radians(degreeRotation.y))
	});
}

glm::mat4 Camera::GetMvpMatrix() const
{
	return GetProjectionMatrix() * GetViewMatrix();
}

void Camera::TestUpdate(glm::vec3 update)
{
	forward = update;
}
