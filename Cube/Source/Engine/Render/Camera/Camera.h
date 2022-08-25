#pragma once

#include <CubeCore.h>
#include <glm/gtc/matrix_transform.hpp>

constexpr float cameraMoveSpeed = 10;
constexpr float cameraSensitivity = 0.2f;
//constexpr float cameraFOV = 45.f;

class Camera 
{
public:

	static Camera* activeCamera;

	float sensitivity;
	float movementSpeed;

	float fov;

	float pitch;
	float yaw;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 up;

	glm::mat4 proj;

	Camera();

	static void SetActiveCamera(Camera* newActiveCamera);

	static void MoveActiveCameraForwards(float deltaTime);
	static void MoveActiveCameraBackwards(float deltaTime);
	static void MoveActiveCameraRight(float deltaTime);
	static void MoveActiveCameraLeft(float deltaTime);
	static void MoveActiveCameraUp(float deltaTime);
	static void MoveActiveCameraDown(float deltaTime);

	glm::mat4 GetMVPMatrix();
};