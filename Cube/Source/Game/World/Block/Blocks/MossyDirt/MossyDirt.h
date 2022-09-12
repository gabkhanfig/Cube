#pragma once

#include <Game/World/Block/BlockTypes/SimpleBlock.h>

class MossyDirt : public SimpleBlock
{
	BLOCK_BODY(BlockId_MossyDirt, "mossy_dirt");

	virtual uint32 GetTextureFaceId(Facing face) override;
};
