#pragma once

#include "../../Engine/EngineCore.h"
#include "../WorldTransform.h"

class Block;

class TerrainGenerator
{
public:

	Block* CreateBlockForWorldPosition(WorldPosition pos) const;
};