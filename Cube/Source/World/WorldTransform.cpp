#include "WorldTransform.h"
#include "Chunk/ChunkData.h"

#pragma region Unit_Test
#ifdef DEVELOPMENT

//consteval bool ChunkPositionTestEqual() {
//	ChunkPosition a = ChunkPosition(10, 11, 12);
//	ChunkPosition b = ChunkPosition(10, 11, 12);
//	return a == b;
//}
//static_assert(ChunkPositionTestEqual(), "[Chunk Position Unit Test]: Chunk positions with the same values are not equal");
//
//consteval bool BlockPositionTestEqual() {
//	BlockPosition a = BlockPosition(10, 11, 12);
//	BlockPosition b = BlockPosition(10, 11, 12);
//	return a == b;
//}
//static_assert(BlockPositionTestEqual(), "[Block Position Unit Test]: Block positions with the same values are not equal");
//
//consteval bool BlockPositionTestBlockIndex() {
//	int index = 0;
//	for (int y = 0; y < CHUNK_LENGTH; y++) {
//		for (int z = 0; z < CHUNK_LENGTH; z++) {
//			for (int x = 0; x < CHUNK_LENGTH; x++) {
//				BlockPosition pos = BlockPosition(x, y, z);
//				if (pos.ToBlockIndex() != index) {
//					return false;
//				}
//				index++;
//			}
//		}
//	}
//	return true;
//}
////static_assert(BlockPositionTestBlockIndex(), "[Block Position Unit Test]: Block position to chunk indices are not correct");
//
//consteval bool BlockPositionTestFromIndex() {
//	int index = 0;
//	for (int y = 0; y < CHUNK_LENGTH; y++) {
//		for (int z = 0; z < CHUNK_LENGTH; z++) {
//			for (int x = 0; x < CHUNK_LENGTH; x++) {
//				if (BlockPosition::FromBlockIndex(index) != BlockPosition(x, y, z)) {
//					return false;
//				}
//				index++;
//			}
//		}
//	}
//	return true;
//}
////static_assert(BlockPositionTestFromIndex(), "[Block Position Unit Test]: Block position from chunk index is not correct");
//
//consteval bool WorldPositionTestEqual() {
//	WorldPosition a = WorldPosition(10, -10, 15);
//	WorldPosition b = WorldPosition(10, -10, 15);
//	return a == b;
//}
//static_assert(WorldPositionTestEqual(), "[World Position Unit Test]: World positions with the same values are not equal");
//
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