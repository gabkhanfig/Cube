#pragma once

#include <EngineCore.h>

class Entity
{
protected:

	glm::dvec3 location;

	glm::dvec3 rotation;

public:

	Entity();

	virtual void Tick(float DeltaTime);

	forceinline void SetLocation(const glm::dvec3 newLocation) { location = newLocation; }

	forceinline void SetRotation(const glm::dvec3 newRotation) { rotation = newRotation; }
};