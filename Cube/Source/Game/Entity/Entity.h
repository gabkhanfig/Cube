#pragma once

#include <CubeCore.h>

class Entity 
{
	friend class Camera;

protected:

	glm::dvec3 position;
	glm::dvec3 previousPosition;
	glm::vec3 rotation;
	glm::vec3 velocity;

	uint8 tickEnabled : 1;

public: 

	inline glm::dvec3 GetPosition() { return position; }
	inline glm::vec3 GetRotation() { return rotation; }

	virtual void Tick(float deltaTime);

};