#include "Camera.h"
#include <Engine/Input/UserInput.h>
#include <iostream>

Camera* Camera::activeCamera;

Camera::Camera()
{
	movementSpeed = cameraMoveSpeed;
	sensitivity = cameraSensitivity;
	fov = 45.f;

	proj = glm::perspective(glm::radians(fov), 16.f / 9.f, 0.1f, 1000.f);

	pitch = 0.f;
	yaw = 0.f;

	position = glm::vec3(0.f, 0.f, 0.f);
	rotation = glm::vec3(pitch, yaw, -1.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::SetActiveCamera(Camera* newActiveCamera)
{
	Camera::activeCamera = newActiveCamera;
	UserInput::setHoldCallback(GLFW_KEY_W, Camera::MoveActiveCameraForwards);
	UserInput::setHoldCallback(GLFW_KEY_S, Camera::MoveActiveCameraBackwards);
	UserInput::setHoldCallback(GLFW_KEY_D, Camera::MoveActiveCameraRight);
	UserInput::setHoldCallback(GLFW_KEY_A, Camera::MoveActiveCameraLeft);
	UserInput::setHoldCallback(GLFW_KEY_SPACE, Camera::MoveActiveCameraUp);
	UserInput::setHoldCallback(GLFW_KEY_LEFT_CONTROL, Camera::MoveActiveCameraDown);
}

void Camera::MoveActiveCameraForwards(float deltaTime)
{
	Camera::activeCamera->position += deltaTime * Camera::activeCamera->movementSpeed * Camera::activeCamera->rotation;
}

void Camera::MoveActiveCameraBackwards(float deltaTime)
{
	Camera::activeCamera->position -= deltaTime * Camera::activeCamera->movementSpeed * Camera::activeCamera->rotation;
}

void Camera::MoveActiveCameraRight(float deltaTime)
{
	Camera::activeCamera->position += float(deltaTime * Camera::activeCamera->movementSpeed) * glm::normalize(glm::cross(Camera::activeCamera->rotation, Camera::activeCamera->up));
}

void Camera::MoveActiveCameraLeft(float deltaTime)
{
	Camera::activeCamera->position -= float(deltaTime * Camera::activeCamera->movementSpeed) * glm::normalize(glm::cross(Camera::activeCamera->rotation, Camera::activeCamera->up));
}

void Camera::MoveActiveCameraUp(float deltaTime)
{
	Camera::activeCamera->position.y += deltaTime * Camera::activeCamera->movementSpeed;
}

void Camera::MoveActiveCameraDown(float deltaTime)
{
	Camera::activeCamera->position.y -= deltaTime * Camera::activeCamera->movementSpeed;
}

glm::mat4 Camera::GetMVPMatrix()
{
	glm::mat4 view;
	view = glm::lookAt(position, position + rotation, up);

	return proj * view;
	//return proj;
}
