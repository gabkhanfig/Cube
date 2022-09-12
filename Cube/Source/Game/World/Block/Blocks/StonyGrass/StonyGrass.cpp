#include "StonyGrass.h"
#pragma once

uint32 StonyGrass::GetTextureFaceId(Facing face)
{
	switch (face) {
	case Facing::Down:
		return BlockTexture_Stone;
	case Facing::North:
		return BlockTexture_StonyGrassSide;
	case Facing::East:
		return BlockTexture_StonyGrassSide;
	case Facing::South:
		return BlockTexture_StonyGrassSide;
	case Facing::West:
		return BlockTexture_StonyGrassSide;
	case Facing::Up:
		return BlockTexture_GrassTop;
	}
}
