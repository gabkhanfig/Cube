#pragma once

#include "../Engine/EngineCore.h"
#include "Chunk/ChunkData.h"

struct BlockFacing
{
public:

	enum Direction : uint8
	{
		Dir_Down = 0b1,
		Dir_Up = 0b10,
		Dir_North = 0b100,
		Dir_South = 0b1000,
		Dir_East = 0b10000,
		Dir_West = 0b100000
	};

	uint8 facing;

	constexpr BlockFacing(uint8 _facing = uint8(BlockFacing::Direction::Dir_Down))
		: facing(_facing)
	{}

	constexpr BlockFacing(BlockFacing::Direction direction)
		: facing(direction)
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
	int index;

	constexpr BlockPosition(int x, int y, int z)
	{
		checkm(x >= 0 && x < CHUNK_LENGTH, "BlockPosition x must be between 0 and CHUNK_LENGTH");
		checkm(y >= 0 && y < CHUNK_LENGTH, "BlockPosition y must be between 0 and CHUNK_LENGTH");
		checkm(z >= 0 && z < CHUNK_LENGTH, "BlockPosition z must be between 0 and CHUNK_LENGTH");
		index = x + (z * CHUNK_LENGTH) + (y * CHUNK_LENGTH * CHUNK_LENGTH);
	}

	constexpr BlockPosition(int _index = 0) {
		checkm(_index >= 0 && _index < CHUNK_SIZE, "BlockPosition index mumst be between 0 and CHUNK_SIZE");
		index = _index;
	}

	forceinline constexpr bool operator == (BlockPosition other) const {
		return index == other.index;
	}

	forceinline constexpr int X() const {
		return index % CHUNK_LENGTH;
	}

	forceinline constexpr int Y() const {
		return index / (CHUNK_LENGTH * CHUNK_LENGTH);
	}

	forceinline constexpr int Z() const {
		return (index % (CHUNK_LENGTH * CHUNK_LENGTH)) / CHUNK_LENGTH;
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
			chunk.x * CHUNK_LENGTH + block.X(),
			chunk.y * CHUNK_LENGTH + block.Y(),
			chunk.z * CHUNK_LENGTH + block.Z()
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

	constexpr BlockPosition ToBlockPosition() const {
		const ChunkPosition cpos = ToChunkPosition();
		return BlockPosition(
			x - (cpos.x * CHUNK_LENGTH),
			y - (cpos.y * CHUNK_LENGTH),
			z - (cpos.z * CHUNK_LENGTH));
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
			return v.index;
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