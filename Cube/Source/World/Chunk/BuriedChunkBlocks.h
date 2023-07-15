#pragma once

#include "../../Engine/EngineCore.h"
#include "../WorldTransform.h"

struct BuriedChunkBlocks {

	__m256i _bitmask[2];

	BuriedChunkBlocks();

	/* Returns a pointer to the start of an 8 element array. */
	[[nodiscard]] const uint64* GetBitmaskAsIntArray() const;

	[[nodiscard]] bool IsBlockBuried(const BlockPosition pos) const;

	void SetBlockBuriedState(const BlockPosition pos, const bool isBuried);

	void Reset();

	[[nodiscard]] bool AreAllBlocksBuried() const;

};
