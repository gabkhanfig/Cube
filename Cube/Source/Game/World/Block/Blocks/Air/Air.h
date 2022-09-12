#pragma once

#include <Game/World/Block/BlockTypes/SimpleBlock.h>

class Air : public SimpleBlock 
{
	BLOCK_BODY(BlockId_Air, "air");

	virtual bool GenerateBlockQuads(Chunk* chunk, const WorldPosition& worldPos, BlockTransform transform, BlockQuad* quadBuffer, uint32& outQuadsGenerated) override;

	virtual inline bool GetIsFaceTransparent(Facing face) override {
		return true;
	}
};