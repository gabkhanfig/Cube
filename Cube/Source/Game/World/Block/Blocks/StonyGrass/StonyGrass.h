#pragma once

#include <Game/World/Block/BlockTypes/SimpleBlock.h>

class StonyGrass : public SimpleBlock
{
	BLOCK_BODY(BlockId_StonyGrass, "stony_grass");

	virtual uint32 GetTextureFaceId(Facing face) override;
};
