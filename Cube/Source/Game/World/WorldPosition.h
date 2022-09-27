#pragma once

#include <CubeCore.h>

#define CHUNK_WIDTH 16

struct ChunkPosition
{
	int x;
	int y;
	int z;

	inline bool operator == (const ChunkPosition& other) const
	{
		bool a = (x == other.x) && (y == other.y) && (z == other.z);
		return a;
	}
};

namespace std
{
	template <>
	struct hash<ChunkPosition> : public unary_function<ChunkPosition, size_t>
	{
		size_t operator()(const ChunkPosition& v) const
		{
			std::size_t h = 0;
			h ^= std::hash<int>{}(v.x) + 0x9e3779b9 + (h << 6) + (h >> 2);
			h ^= std::hash<int>{}(v.y) + 0x9e3779b9 + (h << 6) + (h >> 2);
			h ^= std::hash<int>{}(v.z) + 0x9e3779b9 + (h << 6) + (h >> 2);
			return h;
		}
	};
}

struct BlockPosition
{
	uint8 x;
	uint8 y;
	uint8 z;

	static inline uint32 ToBlockIndex(const BlockPosition block) {
		uint32 index = 0;
		index += block.x;
		index += block.z * CHUNK_WIDTH;
		index += block.y * CHUNK_WIDTH * CHUNK_WIDTH;
		return index;
	}

	static inline BlockPosition FromBlockIndex(const int index) {
		BlockPosition pos;
		pos.x = index % CHUNK_WIDTH;
		pos.y = index / (CHUNK_WIDTH * CHUNK_WIDTH);
		pos.z = (index % (CHUNK_WIDTH * CHUNK_WIDTH)) / CHUNK_WIDTH;
		return pos;
	}
};

struct WorldPosition 
{
	int x;
	int y;
	int z;

	static inline WorldPosition ToWorldPosition(const ChunkPosition& chunk, const BlockPosition block) 
	{
		WorldPosition wp;
		wp.x = chunk.x * CHUNK_WIDTH + int(block.x);
		wp.y = chunk.y * CHUNK_WIDTH + int(block.y);
		wp.z = chunk.z * CHUNK_WIDTH + int(block.z);
		return wp;
	};

	static inline ChunkPosition ToChunkPosition(const WorldPosition& position)
	{
		ChunkPosition pos;
		WorldPosition::ToChunkPosition_Out(pos, position);
		return pos;
	}

	static inline void ToChunkPosition_Out(ChunkPosition& outChunkPosition, const WorldPosition& inWorldPosition)
	{
		outChunkPosition.x = (inWorldPosition.x - (CHUNK_WIDTH - 1) * (inWorldPosition.x < 0)) / CHUNK_WIDTH;
		outChunkPosition.y = (inWorldPosition.y - (CHUNK_WIDTH - 1) * (inWorldPosition.y < 0)) / CHUNK_WIDTH;
		outChunkPosition.z = (inWorldPosition.z - (CHUNK_WIDTH - 1) * (inWorldPosition.z < 0)) / CHUNK_WIDTH;
	}
};




