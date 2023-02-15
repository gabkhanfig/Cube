#pragma once

#include <EngineCore.h>
#include "Chunk/ChunkData.h"

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

struct BlockFacing
{
public:

	enum class Direction : uint8
	{
		Down	= 0b1,
		Up		= 0b10,
		North = 0b100,
		South = 0b1000,
		East	= 0b10000,
		West	= 0b100000
	};

	uint8 facing;

	constexpr BlockFacing(uint8 _facing = uint8(BlockFacing::Direction::Down))
		: facing(_facing)
	{}

};

struct ChunkPosition
{
	int x;
	int y;
	int z;

	constexpr ChunkPosition(int _x = 0, int _y = 0, int _z = 0)
		: x(_x), y(_y), z(_z)
	{}

	forceinline constexpr bool operator == (ChunkPosition other) const	{
		return x == other.x && y == other.y && z == other.z;
	}

};

struct BlockPosition
{
	int x;
	int y;
	int z;

	constexpr BlockPosition(int _x = 0, int _y = 0, int _z = 0)
		: x(_x), y(_y), z(_z)
	{}

	constexpr int ToBlockIndex() const {
		const int index = x + (z * CHUNK_LENGTH) + (y * CHUNK_LENGTH * CHUNK_LENGTH);
		return index;
	}

	static constexpr BlockPosition FromBlockIndex(const int index) {
		BlockPosition pos{
			index % CHUNK_LENGTH,
			index / (CHUNK_LENGTH * CHUNK_LENGTH),
			(index % (CHUNK_LENGTH * CHUNK_LENGTH)) / CHUNK_LENGTH
		};
		return pos;
	}

	forceinline constexpr bool operator == (BlockPosition other) const {
		return x == other.x && y == other.y && z == other.z;
	}

};

struct WorldPosition
{
	int x;
	int y;
	int z;

	constexpr WorldPosition(int _x = 0, int _y = 0, int _z = 0)
		: x(_x), y(_y), z(_z)
	{}

	constexpr WorldPosition(glm::dvec3 pos)
		: x(pos.x), y(pos.y), z(pos.z)
	{}

	static constexpr WorldPosition FromChunkAndBlock(ChunkPosition chunk, BlockPosition block) {
		WorldPosition wp{
			chunk.x * CHUNK_LENGTH + block.x,
			chunk.y * CHUNK_LENGTH + block.y,
			chunk.z * CHUNK_LENGTH + block.z
		};
		return wp;
	}

	constexpr void ToChunkAndBlock(ChunkPosition* outChunk, BlockPosition* outBlock) const {
		const ChunkPosition cpos = ToChunkPosition();
		if (outChunk) *outChunk = cpos;
		if (outBlock) {
			*outBlock = BlockPosition(
			x - (cpos.x * CHUNK_LENGTH),
			y - (cpos.y * CHUNK_LENGTH),
			z - (cpos.z * CHUNK_LENGTH));
		}
		
	}

	constexpr ChunkPosition ToChunkPosition() const {
		ChunkPosition pos;
		pos.x = (x / CHUNK_LENGTH) - (1 * (x < 0));
		pos.y = (y / CHUNK_LENGTH) - (1 * (y < 0));
		pos.z = (z / CHUNK_LENGTH) - (1 * (z < 0));
		return pos;
	}

	forceinline constexpr bool operator == (WorldPosition other) const {
		return x == other.x && y == other.y && z == other.z;
	}

};

namespace std
{
	template <>
	struct hash<ChunkPosition>
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

	template <>
	struct hash<BlockPosition>
	{
		size_t operator()(const BlockPosition& v) const
		{
			std::size_t h = 0;
			h ^= std::hash<int>{}(v.x) + 0x9e3779b9 + (h << 6) + (h >> 2);
			h ^= std::hash<int>{}(v.y) + 0x9e3779b9 + (h << 6) + (h >> 2);
			h ^= std::hash<int>{}(v.z) + 0x9e3779b9 + (h << 6) + (h >> 2);
			return h;
		}
	};

	template <>
	struct hash<WorldPosition>
	{
		size_t operator()(const WorldPosition& v) const
		{
			std::size_t h = 0;
			h ^= std::hash<int>{}(v.x) + 0x9e3779b9 + (h << 6) + (h >> 2);
			h ^= std::hash<int>{}(v.y) + 0x9e3779b9 + (h << 6) + (h >> 2);
			h ^= std::hash<int>{}(v.z) + 0x9e3779b9 + (h << 6) + (h >> 2);
			return h;
		}
	};

}