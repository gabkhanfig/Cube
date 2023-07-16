#pragma once

#include "../../Engine/EngineCore.h"
#include "../WorldTransform.h"

struct BuriedChunkBlocks {

	__m256i _bitmask[2];

	BuriedChunkBlocks();

	BuriedChunkBlocks(const BuriedChunkBlocks& copy);

	/* Returns a pointer to the start of an 8 element array. */
	[[nodiscard]] const uint64* GetBitmaskAsIntArray() const;

	[[nodiscard]] bool IsBlockBuried(const BlockPosition pos) const;

	void SetBlockBuriedState(const BlockPosition pos, const bool isBuried);

	void Reset();

	[[nodiscard]] bool AreAllBlocksBuried() const;

	void operator = (const BuriedChunkBlocks& other);

	[[nodiscard]] bool operator == (const BuriedChunkBlocks& other) const;

};
