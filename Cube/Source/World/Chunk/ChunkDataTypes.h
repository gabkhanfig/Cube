#pragma once

#include "ChunkData.h"
#include "../WorldTransform.h"

class Block;

struct BlockLight
{
	uint8 r;
	uint8 g;
	uint8 b;

	constexpr BlockLight()
		: r(255), g(255), b(255)
	{}

	constexpr BlockLight(uint8 _r, uint8 _g, uint8 _b)
		: r(_r), g(_g), b(_b)
	{}

	/* Linear color range 0.0 - 1.0 */
	constexpr BlockLight(glm::vec3 linearColor) {
		r = uint8(linearColor.r * 255.f);
		g = uint8(linearColor.g * 255.f);
		b = uint8(linearColor.b * 255.f);
	}

	constexpr glm::vec3 ToLinearColor() const {
		return glm::vec3(
			float(r) / 255.f,
			float(g) / 255.f,
			float(b) / 255.f
		);
	}
};