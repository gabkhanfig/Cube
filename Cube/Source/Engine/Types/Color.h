#pragma once

#include "Integers.h"

struct Color 
{
	uint8 r;
	uint8 g;
	uint8 b;
	uint8 a;

	uint32 CombineToInt32() {
		uint32 out = 0;
		out += r;
		out += uint32(g) << 8;
		out += uint32(b) << 16;
		out += uint32(a) << 24;
		return out;
	}
};