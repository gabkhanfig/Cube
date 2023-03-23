#pragma once

#include "../Engine/EngineCore.h"

class IBlock;
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

};