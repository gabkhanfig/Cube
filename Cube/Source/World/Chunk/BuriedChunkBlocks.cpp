#include "BuriedChunkBlocks.h"
#include "Chunk.h"

static_assert(sizeof(BuriedChunkBlocks) * 8 == CHUNK_SIZE, "Buried Chunk Blocks must occupy as many bits as a chunk occupies blocks. See CHUNK_SIZE macro");

BuriedChunkBlocks::BuriedChunkBlocks()
{
	Reset();
}

const uint64* BuriedChunkBlocks::GetBitmaskAsIntArray() const
{
	return reinterpret_cast<const uint64*>(_bitmask->m256i_u64);
}

bool BuriedChunkBlocks::IsBlockBuried(const BlockPosition pos) const
{
	const int arrayIndex = pos.index / 64;
	const int element = pos.index % 64;
	const uint64* array = reinterpret_cast<const uint64*>(_bitmask->m256i_u64);
	return array[arrayIndex] >> element & 1ULL;
}

void BuriedChunkBlocks::SetBlockBuriedState(const BlockPosition pos, const bool isBuried)
{
	const int arrayIndex = pos.index / 64;
	const int element = pos.index % 64;
	uint64* array = reinterpret_cast<uint64*>(_bitmask->m256i_u64);
	array[arrayIndex] ^= (-(static_cast<int64>(isBuried)) ^ array[arrayIndex]) & 1ULL << element;
}

void BuriedChunkBlocks::Reset()
{
	memset(_bitmask, 0, sizeof(BuriedChunkBlocks::_bitmask));
}

bool BuriedChunkBlocks::AreAllBlocksBuried() const
{
	const uint64 allBitsSet[4] = { ~0 };
	const __m256i setBitmask = _mm256_loadu_epi64(allBitsSet);
	return
		_mm256_cmpeq_epi64_mask(_bitmask[0], setBitmask) == 15
		&& _mm256_cmpeq_epi64_mask(_bitmask[1], setBitmask) == 15;
}
