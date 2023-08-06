#pragma once

#include "../../Engine/EngineCore.h"
#include "../WorldTransform.h"
#include "../Block/Block.h"

class TerrainGenerator
{
public:

	Block CreateBlockForWorldPosition(WorldPosition pos) const;
};