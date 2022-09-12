#include "Grass.h"

uint32 Grass::GetTextureFaceId(Facing face)
{
	switch (face) {
	case Facing::Down:
		return BlockTexture_Dirt;
	case Facing::North:
		return BlockTexture_GrassSide;
	case Facing::East:
		return BlockTexture_GrassSide;
	case Facing::South:
		return BlockTexture_GrassSide;
	case Facing::West:
		return BlockTexture_GrassSide;
	case Facing::Up:
		return BlockTexture_GrassTop;
	}
}
