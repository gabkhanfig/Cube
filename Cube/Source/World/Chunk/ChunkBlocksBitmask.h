#pragma once

#include "../../Engine/EngineCore.h"
#include "../WorldTransform.h"

/* 512 bit bitmask able to store flags for an entire chunk of blocks. */
struct ChunkBlocksBitmask {

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

	ChunkBlocksBitmask();

	ChunkBlocksBitmask(const ChunkBlocksBitmask& copy);

	/* Returns a pointer to the start of an 8 element array. */
	[[nodiscard]] const uint64* GetBitmaskAsIntArray() const;

	[[nodiscard]] bool GetFlag(const BlockPosition pos) const;

	void SetFlag(const BlockPosition pos, const bool flag);

	void Reset();

	[[nodiscard]] bool AreAllBlocksSet() const;

	void operator = (const ChunkBlocksBitmask& other);

	[[nodiscard]] bool operator == (const ChunkBlocksBitmask& other) const;

	/* Finds the first set block index within the bitmask using AVX2.
	Don't need to call OptionalIndex::IsValidIndex() if you've already checked AreAllBlocksBuried(). */
	[[nodiscard]] OptionalIndex FirstSetBlockIndex() const;

};
