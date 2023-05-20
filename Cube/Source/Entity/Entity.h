#pragma once

#include "../Engine/EngineCore.h"

class Entity
{
protected:

	/**/
	glm::dvec3 location;

	/**/
	glm::vec3 rotation;

	/* The unit vector of where this entity is looking. */
	glm::vec3 lookAt;

public:

	Entity();

	virtual void Tick(float DeltaTime);

	forceinline glm::dvec3 GetLocation() const { return location; }

	forceinline void SetLocation(const glm::dvec3 newLocation) { location = newLocation; }

	forceinline glm::vec3 GetRotation() const { return rotation; }

	forceinline void SetRotation(const glm::vec3 newRotation) { rotation = newRotation; }

	/* Get the unit vector of where this entity is looking. */
	forceinline glm::vec3 GetLookAt() const { return lookAt; }

	/* Set the unit vector of where this entity is looking. */
	forceinline void SetLookAt(const glm::vec3 newLookAt) { lookAt = newLookAt; }

	forceinline glm::dvec3 GetForwardVector() const { return glm::dvec3(lookAt.x, lookAt.y, lookAt.z); }

	forceinline glm::dvec3 GetRightVector() const { return glm::normalize(glm::cross(GetForwardVector(), glm::dvec3(0, 1, 0))); }

	void AddForwardInput(double amount);
	void AddRightInput(double amount);
	void AddVerticalInput(double amount);

};