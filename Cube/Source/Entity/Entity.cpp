#include "Entity.h"

Entity::Entity()
	: location({ 0, 0, 0 }), rotation({ 0, 0, 0 })
{
}

void Entity::Tick(float DeltaTime)
{
}

ChunkPosition Entity::GetChunkPosition()
{
	return WorldPosition(location).ToChunkPosition();
}

void Entity::AddForwardInput(double amount)
{
	const glm::dvec3 newLocation = location + amount * GetForwardVector();
	location = newLocation;
}

void Entity::AddRightInput(double amount)
{
	const glm::dvec3 newLocation = location + amount * GetRightVector();
	location = newLocation;
}

void Entity::AddVerticalInput(double amount)
{
	const glm::dvec3 newLocation = glm::dvec3(location.x, location.y + amount, location.z);
	location = newLocation;
}
