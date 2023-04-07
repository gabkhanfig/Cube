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

	/* Get the opposite direction of this facing. */
	constexpr BlockFacing Opposite() const {
		const uint8 allDirectionBits = 0b00111111;
		uint8 oppositeBits = 0;
		if (facing & Dir_Down) oppositeBits |= Dir_Up;
		if (facing & Dir_Up) oppositeBits |= Dir_Down;
		if (facing & Dir_North) oppositeBits |= Dir_South;
		if (facing & Dir_South) oppositeBits |= Dir_North;
		if (facing & Dir_East) oppositeBits |= Dir_West;
		if (facing & Dir_West) oppositeBits |= Dir_East;
		return BlockFacing(oppositeBits);
	}

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
		//checkm(x >= 0 && x < CHUNK_LENGTH, "BlockPosition x must be between 0 and CHUNK_LENGTH");
		//checkm(y >= 0 && y < CHUNK_LENGTH, "BlockPosition y must be between 0 and CHUNK_LENGTH");
		//checkm(z >= 0 && z < CHUNK_LENGTH, "BlockPosition z must be between 0 and CHUNK_LENGTH");
		index = x + (z * CHUNK_LENGTH) + (y * CHUNK_LENGTH * CHUNK_LENGTH);
	}

	constexpr BlockPosition(int _index = 0) {
		//checkm(_index >= 0 && _index < CHUNK_SIZE, "BlockPosition index mumst be between 0 and CHUNK_SIZE");
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

/* World position of a block. 
Down = -y,
Up = +y,
North = -z,
South = +z,
East = -x,
West = +x */
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

	constexpr WorldPosition Adjacent(BlockFacing adjacentDirection) const {
		switch (adjacentDirection.facing) {
		case BlockFacing::Direction::Dir_Down:
			return WorldPosition(x, y - 1, z);
		case BlockFacing::Direction::Dir_Up:
			return WorldPosition(x, y + 1, z);
		case BlockFacing::Direction::Dir_North:
			return WorldPosition(x, y, z - 1);
		case BlockFacing::Direction::Dir_South:
			return WorldPosition(x, y, z + 1);
		case BlockFacing::Direction::Dir_East:
			return WorldPosition(x - 1, y, z);
		case BlockFacing::Direction::Dir_West:
			return WorldPosition(x + 1, y, z);
		default:
			checkm(false, "Unreachable code block. BlockFacing face must only be one of the 6 cube faces");
			return *this;
		}
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