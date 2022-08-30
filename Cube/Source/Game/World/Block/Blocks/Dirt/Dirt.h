#pragma once

#include <Game/World/Block/BlockTypes/SimpleBlock.h>

class Dirt : public SimpleBlock 
{
	BLOCK_BODY(BlockId_Dirt, "dirt");

	virtual uint32 GetTextureFaceId(Facing face) override;
};