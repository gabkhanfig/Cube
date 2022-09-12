#pragma once

#include <Game/World/Block/BlockTypes/SimpleBlock.h>

class MossyStone : public SimpleBlock
{
	BLOCK_BODY(BlockId_MossyStone, "mossy_stone");

	virtual uint32 GetTextureFaceId(Facing face) override;
};
