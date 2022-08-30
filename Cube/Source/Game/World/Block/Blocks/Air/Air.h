#pragma once

#include <Game/World/Block/BlockTypes/SimpleBlock.h>

class Air : public SimpleBlock 
{
	BLOCK_BODY(BlockId_Air, "air");

	virtual bool IsBlockAdjacentTransparent(Chunk* owningChunk, WorldPosition position, Facing face) override;

	virtual inline bool GetIsFaceTransparent(Facing face) override {
		return true;
	}
};