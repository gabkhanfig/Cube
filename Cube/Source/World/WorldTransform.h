#pragma once

#include "../Engine/EngineCore.h"
#include "Chunk/ChunkData.h"

/* Only requires 6 bits. */
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

	BlockFacing(uint8 _facing = uint8(BlockFacing::Direction::Dir_Down))
		: facing(_facing)
	{}

	BlockFacing(BlockFacing::Direction direction)
		: facing(direction)
	{}

	BlockFacing(const BlockFacing& other) 
		: facing(other.facing)
	{}

	void operator = (const uint8 bitmask) {
		facing = bitmask;
	}

	void operator = (const BlockFacing other) {
		facing = other.facing;
	}

	bool IsFacing(uint8 direction) const {
		return facing & direction;
	}

	/* Get the opposite direction of this facing. */
	BlockFacing Opposite() const {
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

	ChunkPosition(int _x = 0, int _y = 0, int _z = 0)
		: x(_x), y(_y), z(_z)
	{}

	ChunkPosition(const ChunkPosition& other) 
		: x(other.x), y(other.y), z(other.z)
	{}

	void operator = (const ChunkPosition& other) {
		x = other.x;
		y = other.y;
		z = other.z;
	}

	bool operator == (const ChunkPosition& other) const	{
		return x == other.x && y == other.y && z == other.z;
	}

};

/*  
x has a factor of 1
z has a factor of CHUNK_LENGTH
y has a factor of CHUNK_LENGTH ^ 2
*/
struct BlockPosition
{
	int index;

	BlockPosition(int _index = 0) {
		gk_assertm(_index >= 0 && _index < CHUNK_SIZE, "BlockPosition index must be between 0 and CHUNK_SIZE\nindex: " << _index);
		index = _index;
	}

	BlockPosition(int x, int y, int z)
	{
		gk_assertm(x >= 0 && x < CHUNK_LENGTH, "BlockPosition x must be between 0 and CHUNK_LENGTH");
		gk_assertm(y >= 0 && y < CHUNK_LENGTH, "BlockPosition y must be between 0 and CHUNK_LENGTH");
		gk_assertm(z >= 0 && z < CHUNK_LENGTH, "BlockPosition z must be between 0 and CHUNK_LENGTH");
		index = x + (z * CHUNK_LENGTH) + (y * CHUNK_LENGTH * CHUNK_LENGTH);
	}

	BlockPosition(const BlockPosition& other) 
		: index(other.index)
	{}

	void operator = (const BlockPosition other) {
		index = other.index;
	}

	void operator = (const int _index) {
		gk_assertm(_index >= 0 && _index < CHUNK_SIZE, "BlockPosition index must be between 0 and CHUNK_SIZE\nindex: " << _index);
		index = _index;
	}

	forceinline bool operator == (BlockPosition other) const {
		return index == other.index;
	}

	forceinline int X() const {
		return index % CHUNK_LENGTH;
	}

	forceinline int Y() const {
		return index / (CHUNK_LENGTH * CHUNK_LENGTH);
	}

	forceinline int Z() const {
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

	WorldPosition(int _x = 0, int _y = 0, int _z = 0)
		: x(_x), y(_y), z(_z)
	{}

	WorldPosition(glm::dvec3 pos)
		: x(static_cast<int>(pos.x)), y(static_cast<int>(pos.y)), z(static_cast<int>(pos.z))
	{}

	static WorldPosition FromChunkAndBlock(ChunkPosition chunk, BlockPosition block) {
		WorldPosition wp{
			chunk.x * CHUNK_LENGTH + block.X(),
			chunk.y * CHUNK_LENGTH + block.Y(),
			chunk.z * CHUNK_LENGTH + block.Z()
		};
		return wp;
	}

	ChunkPosition ToChunkPosition() const {
		ChunkPosition pos;
		pos.x = static_cast<int>(floor(double(x) / CHUNK_LENGTH));
		pos.y = static_cast<int>(floor(double(y) / CHUNK_LENGTH));
		pos.z = static_cast<int>(floor(double(z) / CHUNK_LENGTH));
		return pos;
	}

	BlockPosition ToBlockPosition() const {
		const ChunkPosition cpos = ToChunkPosition();
		return BlockPosition(
			x - (cpos.x * CHUNK_LENGTH),
			y - (cpos.y * CHUNK_LENGTH),
			z - (cpos.z * CHUNK_LENGTH));
	}

	forceinline bool operator == (WorldPosition other) const {
		return x == other.x && y == other.y && z == other.z;
	}

	WorldPosition Adjacent(BlockFacing adjacentDirection) const {
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
			gk_assertm(false, "Unreachable code block. BlockFacing face must only be one of the 6 cube faces");
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

template<>
[[nodiscard]] inline static gk::String gk::String::From<ChunkPosition>(const ChunkPosition& value) {
	return "x: " + gk::String::FromInt(value.x) + ", y: " + gk::String::FromInt(value.y) + ", z: " + gk::String::FromInt(value.z);
}

template<>
[[nodiscard]] inline static gk::String gk::String::From<BlockPosition>(const BlockPosition& value) {
	return "x: " + gk::String::FromInt(value.X()) + ", y: " + gk::String::FromInt(value.Y()) + ", z: " + gk::String::FromInt(value.Z());
}

template<>
[[nodiscard]] inline static gk::String gk::String::From<WorldPosition>(const WorldPosition& value) {
	return "x: " + gk::String::FromInt(value.x) + ", y: " + gk::String::FromInt(value.y) + ", z: " + gk::String::FromInt(value.z);
}