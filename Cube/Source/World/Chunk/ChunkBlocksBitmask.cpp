#include "ChunkBlocksBitmask.h"
#include "Chunk.h"

//static_assert(sizeof(ChunkBlocksBitmask) * 8 == CHUNK_SIZE, "Buried Chunk Blocks must occupy as many bits as a chunk occupies blocks. See CHUNK_SIZE macro");

ChunkBlocksBitmask::ChunkBlocksBitmask()
{
	Reset();
}

ChunkBlocksBitmask::ChunkBlocksBitmask(const ChunkBlocksBitmask& copy)
{
	memcpy(_bitmask, copy._bitmask, sizeof(ChunkBlocksBitmask::_bitmask));
}

const uint64* ChunkBlocksBitmask::GetBitmaskAsIntArray() const
{
	//return reinterpret_cast<const uint64*>(_bitmask->m256i_u64);
	return _bitmask;
}

bool ChunkBlocksBitmask::GetFlag(const BlockPosition pos) const
{
	const int arrayIndex = pos.index / 64;
	const int element = pos.index % 64;
	//const uint64* array = reinterpret_cast<const uint64*>(_bitmask->m256i_u64);
	return _bitmask[arrayIndex] >> element & 1ULL;
}

void ChunkBlocksBitmask::SetFlag(const BlockPosition pos, const bool flag)
{
	const int arrayIndex = pos.index / 64;
	const int element = pos.index % 64;
	//uint64* array = reinterpret_cast<uint64*>(_bitmask->m256i_u64);
	_bitmask[arrayIndex] ^= (-(static_cast<int64>(flag)) ^ _bitmask[arrayIndex]) & 1ULL << element;
}

void ChunkBlocksBitmask::Reset()
{
	memset(_bitmask, 0, sizeof(ChunkBlocksBitmask::_bitmask));
}

bool ChunkBlocksBitmask::AreAllBlocksSet() const
{
	//const uint64 allBitsSet[4] = { ~0ULL, ~0ULL, ~0ULL, ~0ULL };
	//const __m256i setBitmask = _mm256_loadu_epi64(allBitsSet);
	//const __m256i setBitmask = _mm256_set1_epi64x(~0ULL);
	//for (int i = 0; i < 128; i++) {
	//	if (_mm256_cmpeq_epi64_mask(_bitmask[0], setBitmask) != 15) return false;
	//}
	//return true;
	for (int i = 0; i < 512; i++) {
		if (_bitmask[i] != (~0ULL)) return false;
	}
	return true;

	//return
	//	_mm256_cmpeq_epi64_mask(_bitmask[0], setBitmask) == 15
	//	&& _mm256_cmpeq_epi64_mask(_bitmask[1], setBitmask) == 15;
}

void ChunkBlocksBitmask::SetAllFlags()
{
	memset(_bitmask, 255, sizeof(ChunkBlocksBitmask::_bitmask));
}

void ChunkBlocksBitmask::operator=(const ChunkBlocksBitmask& other)
{
	memcpy(_bitmask, other._bitmask, sizeof(ChunkBlocksBitmask::_bitmask));
}

bool ChunkBlocksBitmask::operator==(const ChunkBlocksBitmask& other) const
{
	//for (int i = 0; i < 128; i++) {
	//	if(_mm256_cmpeq_epi64_mask(_bitmask[i], other._bitmask[i]) != 15) return false;
	//}
	//return true;
	//return _mm256_cmpeq_epi64_mask(_bitmask[0], other._bitmask[0]) == 15
	//	&& _mm256_cmpeq_epi64_mask(_bitmask[1], other._bitmask[1]) == 15;
	for (int i = 0; i < 512; i++) {
		if (_bitmask[i] != other._bitmask[i]) return false;
	}
	return true;
}

#pragma intrinsic(_BitScanForward)

ChunkBlocksBitmask::OptionalIndex ChunkBlocksBitmask::FirstSetBlockIndex() const
{
	gk_assertm(false, "not implemented");
	return OptionalIndex();
	/*
	const uint64 notSet[4] = { 64, 64, 64, 64 };
	const __m256i notSetVector = _mm256_loadu_epi64(notSet); // Pre-load vector with all epi64 elements set to 64 for comparison checks later.

	const __m256i results[2] = { // Get lowest non zero bit offset count. Each uint64 element is 64 if it's all zero.
		_mm256_lzcnt_epi64(_bitmask[0]), 
		_mm256_lzcnt_epi64(_bitmask[1])
	};
	const uint64* asArray = reinterpret_cast<const uint64*>(results);

	const uint8 mask1 = _mm256_cmpneq_epi64_mask(results[0], notSetVector); // Compare not equal mask, for if any epi64 elements arent equal to 64 (thus have a set bit).
	const uint8 mask2 = _mm256_cmpneq_epi64_mask(results[1], notSetVector);
	unsigned long toCheck;
	if(_BitScanForward(&toCheck, mask1 | (mask2 << 4)) == 0) return OptionalIndex(); // If none of the bits are set from the comparison, return an optional index with an invalid index.
	return OptionalIndex(static_cast<int>((63 - asArray[toCheck]) + ((toCheck) * 64))); // Otherwise, return the index.*/
}

#pragma intrinsic(_BitScanForward64)

ChunkBlocksBitmask::OptionalIndex ChunkBlocksBitmask::FirstNonSetBlockIndex() const
{
	gk_assertm(false, "not implemented");
	const uint64* asArray = GetBitmaskAsIntArray();
	int step = 0;
	for (int i = 0; i < 8; i++) {
		unsigned long toCheck;
		if (_BitScanForward64(&toCheck, ~asArray[i]) == 0) {
			step += 64;
			continue;
		}
		return toCheck + step;
	}
	return OptionalIndex();
	

	//const __m256i allBitsSetVec = _mm256_set1_epi64x(~0ULL);
	//const __m256i notSetVector = _mm256_set1_epi64x(64);

	//const __m256i results[2] = {
	//	_mm256_lzcnt_epi64(_mm256_andnot_epi64(_bitmask[0], allBitsSetVec)),
	//	_mm256_lzcnt_epi64(_mm256_andnot_epi64(_bitmask[1], allBitsSetVec))
	//};
	//
	//const uint64* asArray = reinterpret_cast<const uint64*>(results);

	//const uint8 mask1 = _mm256_cmpneq_epi64_mask(results[0], notSetVector); // Compare not equal mask, for if any epi64 elements arent equal to 64 (thus have a set bit).
	//const uint8 mask2 = _mm256_cmpneq_epi64_mask(results[1], notSetVector);
	//unsigned long toCheck;
	//if (_BitScanForward(&toCheck, mask1 | (mask2 << 4)) == 0) return OptionalIndex(); // If none of the bits are set from the comparison, return an optional index with an invalid index.
	//return OptionalIndex((63 - asArray[toCheck]) + ((toCheck) * 64)); // Otherwise, return the index.
}
