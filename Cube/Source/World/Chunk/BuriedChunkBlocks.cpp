#include "BuriedChunkBlocks.h"
#include "Chunk.h"

static_assert(sizeof(BuriedChunkBlocks) * 8 == CHUNK_SIZE, "Buried Chunk Blocks must occupy as many bits as a chunk occupies blocks. See CHUNK_SIZE macro");

BuriedChunkBlocks::BuriedChunkBlocks()
{
	Reset();
}

bool BuriedChunkBlocks::IsBlockBuried(const BlockPosition pos) const
{
	const int arrayIndex = pos.index / 64;
	const int element = pos.index % 64;
	const uint64* array = reinterpret_cast<const uint64*>(_bitmask->m256i_i64);
	return array[arrayIndex] >> element & 1ULL;
}

void BuriedChunkBlocks::SetBlockBuriedState(const BlockPosition pos, const bool isBuried)
{
	const int arrayIndex = pos.index / 64;
	const int element = pos.index % 64;
	uint64* array = reinterpret_cast<uint64*>(_bitmask->m256i_i64);
	array[arrayIndex] ^= (-(static_cast<int64>(isBuried)) ^ array[arrayIndex]) & 1ULL << element;
}

void BuriedChunkBlocks::Reset()
{
	memset(_bitmask, 0, sizeof(BuriedChunkBlocks::_bitmask));
}