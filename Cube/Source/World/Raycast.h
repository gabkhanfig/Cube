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

	HitSuccess success;
	Block* hitBlock;
	glm::dvec3 position;
	glm::dvec3 normal;

	RaycastHitResult() :
		success(HitSuccess::none),
		hitBlock(nullptr) {}

};