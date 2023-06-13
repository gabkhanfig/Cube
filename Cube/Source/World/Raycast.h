#pragma once

#include "../Engine/EngineCore.h"

class Block;
class Entity;
class World;

struct RaycastHitResult
{
public:

	enum class HitSuccess : uint8 {
		/* Did not hit anything */ 
		none,
		/* Hit a block */ 
		block,
		/* Hit an entity */ 
		entity
	};

	RaycastHitResult() :
		success(HitSuccess::none),
		hitObject(nullptr),
		position(0, 0, 0),
		normal(0, 1, 0)
	{}

	Block* GetHitBlock() const;
	Entity* GetHitEntity() const;

public:

	HitSuccess success;
	IObject* hitObject;
	glm::dvec3 position;
	glm::dvec3 normal;

};