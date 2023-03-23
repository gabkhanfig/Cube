#include "WorldTransform.h"
#include "Chunk/ChunkData.h"

#pragma region Unit_Test
#ifdef DEVELOPMENT

consteval bool BlockFacingTestOppositeDown() {
	BlockFacing b = BlockFacing::Direction::Dir_Down;
	return b.Opposite().facing == BlockFacing::Direction::Dir_Up;
}
static_assert(BlockFacingTestOppositeDown(), "[Block Facing Unit Test]: Block Facing opposite of down must be up");

consteval bool BlockFacingTestOppositeUp() {
	BlockFacing b = BlockFacing::Direction::Dir_Up;
	return b.Opposite().facing == BlockFacing::Direction::Dir_Down;
}
static_assert(BlockFacingTestOppositeUp(), "[Block Facing Unit Test]: Block Facing opposite of up must be down");

consteval bool BlockFacingTestOppositeNorth() {
	BlockFacing b = BlockFacing::Direction::Dir_North;
	return b.Opposite().facing == BlockFacing::Direction::Dir_South;
}
static_assert(BlockFacingTestOppositeNorth(), "[Block Facing Unit Test]: Block Facing opposite of north must be south");

consteval bool BlockFacingTestOppositeSouth() {
	BlockFacing b = BlockFacing::Direction::Dir_South;
	return b.Opposite().facing == BlockFacing::Direction::Dir_North;
}
static_assert(BlockFacingTestOppositeSouth(), "[Block Facing Unit Test]: Block Facing opposite of south must be north");

consteval bool BlockFacingTestOppositeEast() {
	BlockFacing b = BlockFacing::Direction::Dir_East;
	return b.Opposite().facing == BlockFacing::Direction::Dir_West;
}
static_assert(BlockFacingTestOppositeEast(), "[Block Facing Unit Test]: Block Facing opposite of east must be west");

consteval bool BlockFacingTestOppositeWest() {
	BlockFacing b = BlockFacing::Direction::Dir_West;
	return b.Opposite().facing == BlockFacing::Direction::Dir_East;
}
static_assert(BlockFacingTestOppositeWest(), "[Block Facing Unit Test]: Block Facing opposite of west must be east");

consteval bool ChunkPositionTestEqual() {
	ChunkPosition a = ChunkPosition(10, 11, 12);
	ChunkPosition b = ChunkPosition(10, 11, 12);
	return a == b;
}
static_assert(ChunkPositionTestEqual(), "[Chunk Position Unit Test]: Chunk positions with the same values are not equal");

consteval bool BlockPositionTestEqual() {
	BlockPosition a = BlockPosition(10, 11, 12);
	BlockPosition b = BlockPosition(10, 11, 12);
	return a == b;
}
static_assert(BlockPositionTestEqual(), "[Block Position Unit Test]: Block positions with the same values are not equal");

consteval bool WorldPositionTestEqual() {
	WorldPosition a = WorldPosition(10, -10, 15);
	WorldPosition b = WorldPosition(10, -10, 15);
	return a == b;
}
static_assert(WorldPositionTestEqual(), "[World Position Unit Test]: World positions with the same values are not equal");

// NOTE: With large CHUNK_LENGTH, the consteval function will be too large to be executed.
consteval bool BlockPositionTestBlockIndex() {
	int index = 0;
	for (int y = 0; y < CHUNK_LENGTH; y++) {
		for (int z = 0; z < CHUNK_LENGTH; z++) {
			for (int x = 0; x < CHUNK_LENGTH; x++) {
				BlockPosition pos = BlockPosition(x, y, z);
				if (pos.index != index) {
					return false;
				}
				index++;
			}
		}
	}
	return true;
}
static_assert(BlockPositionTestBlockIndex(), "[Block Position Unit Test]: Block position to chunk indices are not correct");

// NOTE: With large CHUNK_LENGTH, the consteval function will be too large to be executed.
consteval bool BlockPositionTestFromIndex() {
	int index = 0;
	for (int y = 0; y < CHUNK_LENGTH; y++) {
		for (int z = 0; z < CHUNK_LENGTH; z++) {
			for (int x = 0; x < CHUNK_LENGTH; x++) {
				if (BlockPosition(index) != BlockPosition(x, y, z)) {
					return false;
				}
				index++;
			}
		}
	}
	return true;
}
static_assert(BlockPositionTestFromIndex(), "[Block Position Unit Test]: Block position from chunk index is not correct");

//consteval bool WorldPositionTestFromChunkAndBlock() {
//	ChunkPosition c{ 2, 0, -3 };
//	BlockPosition b{ 1, 4, 7 };
//	WorldPosition pos = WorldPosition::FromChunkAndBlock(c, b);
//	return pos == WorldPosition(2 * CHUNK_LENGTH + 1, 0 * CHUNK_LENGTH + 4, -3 * CHUNK_LENGTH + 7);
//}
//static_assert(WorldPositionTestFromChunkAndBlock(), "[World Position Unit Test]: World position from chunk and block positions is incorrect");
//
//consteval bool WorldPositionTestToChunk() {
//	WorldPosition pos = WorldPosition(65, 4, -15);
//	ChunkPosition c = pos.ToChunkPosition();
//	return c == ChunkPosition(1, 0, -1);
//}
////static_assert(WorldPositionTestToChunk(), "[World Position Unit Test]: Conversion to chunk position is incorrect");
//
//consteval bool WorldPositionTestToBlock() {
//	WorldPosition pos = WorldPosition(33, 4, -41);
//	ChunkPosition c;
//	BlockPosition b;
//	pos.ToChunkAndBlock(&c, &b);
//	return b == BlockPosition(33, 4, 23);
//}
////static_assert(WorldPositionTestToBlock(), "[World Position Unit Test]: Conversion to block position is incorrect");

#endif
#pragma endregion