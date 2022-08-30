#pragma once

#include <Game/World/Block/BlockTypes/SimpleBlock.h>

class Stone : public SimpleBlock
{
	BLOCK_BODY(BlockId_Stone, "stone");

	virtual uint32 GetTextureFaceId(Facing face) override;
};
