#pragma once

#include "../Engine/EngineCore.h"
#include "../World/WorldTransform.h"

class Entity : public IObject
{
public:

	Entity();

	virtual void Tick(float DeltaTime);

	forceinline glm::dvec3 GetLocation() const { return location; }

	forceinline void SetLocation(const glm::dvec3 newLocation) { location = newLocation; }

	forceinline glm::dvec3 GetRotation() const { return rotation; }

	forceinline void SetRotation(const glm::dvec3 newRotation) { rotation = newRotation; }

	forceinline glm::dvec3 GetForwardVector() const { return glm::normalize(glm::cross(GetRightVector(), glm::dvec3(0, -1, 0))); }

	forceinline glm::dvec3 GetRightVector() const { return glm::normalize(glm::cross(GetRotation(), glm::dvec3(0, 1, 0))); }

	ChunkPosition GetChunkPosition();

	void AddForwardInput(double amount);
	void AddRightInput(double amount);
	void AddVerticalInput(double amount);

#pragma region IObject_Implementation

	virtual bool IsPendingDelete() const final { return isPendingDelete; }
	virtual void _SetIsPendingDelete() final { isPendingDelete = true; }

#pragma endregion

protected:

	/**/
	glm::dvec3 location;

	/**/
	glm::dvec3 rotation;

private:

	bool isPendingDelete;

};