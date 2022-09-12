#pragma once

#include <CubeCore.h>

class Entity 
{
	friend class Camera;

protected:

	glm::dvec3 position;
	glm::dvec3 rotation;
	glm::dvec3 velocity;

	uint8 tickEnabled : 1;

public:

	inline glm::dvec3 GetPosition() { return position; }

};