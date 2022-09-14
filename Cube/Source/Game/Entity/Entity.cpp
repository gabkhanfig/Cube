#include "Entity.h"

void Entity::Tick(float deltaTime)
{
	position += deltaTime * velocity;
}
