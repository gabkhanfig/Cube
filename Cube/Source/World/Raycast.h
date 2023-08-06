#pragma once

#include "../Engine/EngineCore.h"
#include "Block/Block.h"

class Entity;
class World;

struct RaycastHitResult
{
public:

	enum class HitSuccess : uint8 {
		/* Did not hit anything */ 
		None,
		/* Hit a block */ 
		Block,
		/* Hit an entity */ 
		Entity
	};

	RaycastHitResult() :
		success(HitSuccess::None),
		hitObject(nullptr),
		position(0, 0, 0),
		normal(0, 1, 0)
	{}

	/* Get the hit object as a block. Returns nullptr if it did not hit a block. */
	Block* GetHitBlock() const;

	/* Get the hit object as an entity. Returns nullptr if it did not hit an entity. */
	Entity* GetHitEntity() const;

	

public:

	HitSuccess success;
	IObject* hitObject;
	glm::dvec3 position;
	glm::dvec3 normal;
	Chunk* hitChunk;
	BlockPosition hitBpos;

};