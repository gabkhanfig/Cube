#pragma once

#include "../../IBlock.h"

class StoneBlock : public IBlock
{
	BLOCK_BODY(StoneBlock, "stoneBlock");
	CUBE_MESH(EBlockTexture::stone);
};