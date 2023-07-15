#pragma once

#include "../../Engine/EngineCore.h"
#include "../WorldTransform.h"

struct BuriedChunkBlocks {

	__m256i _bitmask[2];

	BuriedChunkBlocks();

	[[nodiscard]] bool IsBlockBuried(const BlockPosition pos) const;

	void SetBlockBuriedState(const BlockPosition pos, const bool isBuried);

	void Reset();

};
