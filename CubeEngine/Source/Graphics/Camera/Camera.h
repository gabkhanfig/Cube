#pragma once

#include "../../EngineCore.h"
#include <glm/gtc/matrix_transform.hpp>

constexpr double cameraSensitivity = 0.2;
constexpr float cameraFOV = 90.f;
constexpr int chunkRenderDistance = 15;

class Camera
{
private:

	static Camera* activeCamera;

	glm::vec3 forward;

	glm::vec3 position;

public:

	forceinline glm::vec3 GetForward() const { return forward; }

private:

	glm::mat4 GetProjectionMatrix() const;

	glm::mat4 GetViewMatrix() const;

public:
	
	Camera();

	void Bind();

	void Unbind();

	static Camera* GetActiveCamera();

	bool IsBound() const;

	glm::mat4 GetMvpMatrix() const;

	static constexpr glm::vec3 GetUpVector() { return glm::vec3(0.0f, 1.0f, 0.0f); }
	
	void SetForwardVector(glm::vec3 update);

	void SetPosition(glm::vec3 newPosition);

};