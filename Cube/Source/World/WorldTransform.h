#pragma once

#include <EngineCore.h>

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
	uint8 x;
	uint8 y;
	uint8 z;

	constexpr BlockPosition(uint8 _x = 0, uint8 _y = 0, uint8 _z = 0)
		: x(_x), y(_y), z(_z)
	{}

	constexpr uint16 ToBlockIndex() const {
		const uint16 index = x + (z * 16) + (y * 16 * 16);
		return index;
	}

	static constexpr BlockPosition FromBlockIndex(const uint16 index) {
		BlockPosition pos{
			uint8(index % 16), 
			uint8(index / (16 * 16)),
			uint8((index % (16 * 16)) / 16)
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
			(chunk.x * 16) + block.x,
			chunk.y * 16 + int(block.y),
			chunk.z * 16 + int(block.z)
		};
		return wp;
	}

	constexpr void ToChunkAndBlock(ChunkPosition* outChunk, BlockPosition* outBlock) const {
		const ChunkPosition cpos = ToChunkPosition();
		if (outChunk) *outChunk = cpos;
		if (outBlock) {
			*outBlock = BlockPosition(
			x - (cpos.x * 16),
			y - (cpos.y * 16),
			z - (cpos.z * 16));
		}
		
	}

	constexpr ChunkPosition ToChunkPosition() const {
		ChunkPosition pos;
		pos.x = (x / 16) - (1 * (x < 0));
		pos.y = (y / 16) - (1 * (y < 0));
		pos.z = (z / 16) - (1 * (z < 0));
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