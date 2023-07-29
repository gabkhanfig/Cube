#pragma once

#include "../../Core/EngineTypes.h"

// 10 bits
constexpr uint32 _packedComponentBitmask = 1023U;

/* 3d vector with normalized components ranging -1 -> 1 compressed into a singular unsigned integer. 
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
