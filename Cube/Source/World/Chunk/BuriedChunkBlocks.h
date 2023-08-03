#pragma once

#include "../../Engine/EngineCore.h"
#include "../WorldTransform.h"

struct BuriedChunkBlocks {

	struct OptionalIndex {
		OptionalIndex() : _index(MAXINT32) {}

		OptionalIndex(int index) : _index(index) {}

		forceinline bool IsValidIndex() const {
			return _index != MAXINT32;
		}

		forceinline int Get() const {
			gk_assertm(IsValidIndex(), "Buried Chunk Blocks result optional index is not valid.");
			return _index;
		}

	private:
		int _index;
	};

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

	/* Don't need to call OptionalIndex::IsValidIndex() if you've already checked AreAllBlocksBuried(). */
	[[nodiscard]] OptionalIndex FirstSetBlockIndex() const;

};
