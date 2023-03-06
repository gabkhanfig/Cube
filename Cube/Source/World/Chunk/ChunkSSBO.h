#pragma once

#include "../../Engine/EngineCore.h"
#include "ChunkData.h"

#define COLORS_COUNT 17 * 17 * 17

struct ChunkSSBO
{
	int position[3];
	uint32 colors[COLORS_COUNT];
	float textures[CHUNK_SIZE];

	ChunkSSBO() 
	{
		memset(position, 0, sizeof(position));
		memset(colors, 0, sizeof(colors));
		memset(textures, 0, sizeof(textures));
	}

	ChunkSSBO(int _position[3], uint32 _colors[COLORS_COUNT], float _textures[CHUNK_SIZE])
	{
		memcpy(position, _position, sizeof(position));
		memcpy(colors, _colors, sizeof(colors));
		memcpy(textures, _textures, sizeof(textures));
	}
};
static_assert(sizeof(ChunkSSBO) ==
	sizeof(int[3]) + sizeof(uint32[COLORS_COUNT]) + sizeof(float[CHUNK_SIZE])
	, "ChunkSSBO is not tightly packed");