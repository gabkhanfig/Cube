#pragma once

#include "../../../Engine/EngineCore.h"
#include <glm/gtc/matrix_transform.hpp>

constexpr float cameraFOV = 90.f;
constexpr int chunkRenderDistance = 15;

class Camera
{
private:

	static Camera* activeCamera;

	glm::dvec3* forward;

public:

	forceinline const glm::dvec3 GetForward() const { return *forward; }

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

	void SetForwardVectorReference(glm::dvec3* newForward) { forward = newForward; }

};