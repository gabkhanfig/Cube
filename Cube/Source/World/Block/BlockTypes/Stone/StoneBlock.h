#pragma once

#include "../../Block.h"

class StoneBlock : public Block
{
	BLOCK_BODY(StoneBlock, "stoneBlock");
	CUBE_MESH(EBlockTexture::stone);
};