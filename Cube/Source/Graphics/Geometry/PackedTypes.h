#pragma once

#include "../../Core/EngineTypes.h"
#include "../../World/Chunk/ChunkData.h"

// 10 bits
constexpr uint32 _packedComponentBitmask = 1023U;

/* 3d position vector representing the offset of a block's vertex position within a chunk (and adjacent ones) compressed into a singular unsigned integer.
Each XYZ component gets 10 bits. The first 8 represent the coordinate within the chunk. 
The 9th bit represents whether it's to be shifted into the adjacent chunk with a negative offset.
The 10th bit represents whether it's to be shifted into the adjacent chunk with a positive offset.
Can represent sub-block-pixels of 16 (16x16 textures / models).
NOTE: only works if chunk length is 8. */
struct PackedBlockOffsetPosition
{
	static_assert(CHUNK_LENGTH == 8);

	uint32 packedData;
	static constexpr float SCALAR = 16.f;

	/* Will assert of any of the position XYZ components is not within the inclusive range of -8 -> 16. */
	static PackedBlockOffsetPosition Pack(const glm::vec3 pos) {
		gk_assertm(pos.x >= -8 && pos.x <= 16, "Cannot pack a block offset position with ax component not within the bounds -8 -> 16\nx: " << pos.x);
		gk_assertm(pos.y >= -8 && pos.y <= 16, "Cannot pack a block offset position that an y component not within the bounds -8 -> 16\nx: " << pos.y);
		gk_assertm(pos.z >= -8 && pos.z <= 16, "Cannot pack a block offset position that an z component not within the bounds -8 -> 16\nx: " << pos.z);
		const uint32 x = _PackFloat(pos.x);
		const uint32 y = _PackFloat(pos.y);
		const uint32 z = _PackFloat(pos.z); 
		const uint32 data = x | (y << 10) | (z << 20);
		return { data };
	}

	glm::vec3 Unpack() const {
		const uint32 xBitmask = packedData & _packedComponentBitmask;
		const uint32 yBitmask = (packedData >> 10) & _packedComponentBitmask;
		const uint32 zBitmask = (packedData >> 20) & _packedComponentBitmask;
		return glm::vec3(_UnpackFloat(xBitmask), _UnpackFloat(yBitmask), _UnpackFloat(zBitmask));
	}

	static uint32 _PackFloat(float n) {
		const bool isNegativeExtension = n < 0;
		const bool isPositiveExtension = n > CHUNK_LENGTH;
		const uint32 extensionBitmask = (static_cast<uint32>(isNegativeExtension) << 8) | (static_cast<uint32>(isPositiveExtension) << 9);

		const float negativeShift = static_cast<float>(isNegativeExtension) * CHUNK_LENGTH * -1;
		const float positiveShift = static_cast<float>(isPositiveExtension) * CHUNK_LENGTH;
		const float withinChunkBound = n - negativeShift - positiveShift;

		const uint32 packed = static_cast<uint32>(withinChunkBound * SCALAR);
		return packed | extensionBitmask;
	}

	static float _UnpackFloat(uint32 bitmask) {
		const float negativeShift = static_cast<float>((bitmask >> 8) & 1) * CHUNK_LENGTH * -1; // 9th bit is negative offset
		const float positiveShift = static_cast<float>((bitmask >> 9) & 1) * CHUNK_LENGTH; // 10th bit is positive offset
		const float withinChunkBound = static_cast<float>(bitmask & 0b11111111) / SCALAR;
		return withinChunkBound + negativeShift + positiveShift;
	}
};

/* 3d direction vector with normalized components ranging -1 -> 1 compressed into a singular unsigned integer. 
It is lossless at component (xyz) = -1, 0, or 1. */
struct PackedNormal
{
	uint32 packedData;
	static constexpr float SCALAR = 511.f;

	/* Will assert when normal is non-normalized (length of 1). See glm::normalize. */
	static PackedNormal Pack(const glm::vec3 normal) {
		gk_assertm(IsNearlyEqual(glm::length(normal), 1.f), "Cannot pack a normal vector who's length is not nearly equal to 1\nNormal: " << String::From(normal) << "\nLength: " << glm::length(normal));
		const uint32 x = static_cast<uint32>(normal.x * SCALAR + SCALAR);
		const uint32 y = static_cast<uint32>(normal.y * SCALAR + SCALAR);
		const uint32 z = static_cast<uint32>(normal.z * SCALAR + SCALAR);
		const uint32 data = x | (y << 10) | (z << 20);
		return { data };
	}

	glm::vec3 Unpack() const {
		const float x = (static_cast<float>(packedData & _packedComponentBitmask) - SCALAR) / SCALAR;
		const float y = (static_cast<float>((packedData >> 10) & _packedComponentBitmask) - SCALAR) / SCALAR;
		const float z = (static_cast<float>((packedData >> 20) & _packedComponentBitmask) - SCALAR) / SCALAR; 
		return glm::vec3(x, y, z);
	}
};
