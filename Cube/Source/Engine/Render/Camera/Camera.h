#pragma once

#include <CubeCore.h>
#include <glm/gtc/matrix_transform.hpp>

constexpr float cameraSensitivity = 0.2f;
constexpr float cameraFOV = 45.f;

class Entity;

class Camera 
{
public:

	static Camera* activeCamera;

	Entity* followEntity;

	float sensitivity;

	float fov;

	float pitch;
	float yaw;


	glm::mat4 proj;

	Camera();

	static void SetActiveCamera(Camera* newActiveCamera);
	inline static Camera* GetActiveCamera() { return activeCamera; }

	void SetRotation(const glm::vec3& newRotation);
	void SetFollowingEntity(Entity* entity);

	constexpr glm::vec3 GetUpVector() { return glm::vec3(0.0f, 1.0f, 0.0f); }

	glm::mat4 GetMVPMatrix();
};