#pragma once

#include <Game/World/Block/BlockTypes/SimpleBlock.h>

class Grass : public SimpleBlock
{
	BLOCK_BODY(BlockId_Grass, "grass");

	virtual uint32 GetTextureFaceId(Facing face) override;
};