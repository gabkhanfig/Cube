#pragma once

#include "../Engine/EngineCore.h"

class Entity
{
protected:

	/**/
	glm::dvec3 location;

	/**/
	glm::dvec3 rotation;

public:

	Entity();

	virtual void Tick(float DeltaTime);

	forceinline glm::dvec3 GetLocation() const { return location; }

	forceinline void SetLocation(const glm::dvec3 newLocation) { location = newLocation; }

	forceinline glm::dvec3 GetRotation() const { return rotation; }

	forceinline void SetRotation(const glm::dvec3 newRotation) { rotation = newRotation; }

	forceinline glm::dvec3 GetForwardVector() const { return glm::normalize(glm::cross(GetRightVector(), glm::dvec3(0, -1, 0))); }

	forceinline glm::dvec3 GetRightVector() const { return glm::normalize(glm::cross(GetRotation(), glm::dvec3(0, 1, 0))); }

	void AddForwardInput(double amount);
	void AddRightInput(double amount);
	void AddVerticalInput(double amount);

};