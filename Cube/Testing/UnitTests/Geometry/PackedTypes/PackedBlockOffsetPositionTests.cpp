#include "../../../CubeTest.h"
#include "../../../../Source/Graphics/Geometry/PackedTypes.h"

#if WITH_TESTS

namespace UnitTests
{
	TEST(PackedBlockOffsetPositionTest, PackZeroNoAssert) {
		const glm::vec3 zero{0, 0, 0};
		PackedBlockOffsetPosition packed = PackedBlockOffsetPosition::Pack(zero);
	}

	TEST(PackedBlockOffsetPositionTest, PackChunkLengthNoAssert) {
		const glm::vec3 chunkLen{CHUNK_LENGTH, CHUNK_LENGTH, CHUNK_LENGTH};
		PackedBlockOffsetPosition packed = PackedBlockOffsetPosition::Pack(chunkLen);
	}

	TEST(PackedBlockOffsetPositionTest, PackAdjacentChunkNegativeNoAssert) {
		const glm::vec3 chunkNegative{-CHUNK_LENGTH, -CHUNK_LENGTH, -CHUNK_LENGTH};
		PackedBlockOffsetPosition packed = PackedBlockOffsetPosition::Pack(chunkNegative);
	}

	TEST(PackedBlockOffsetPositionTest, PackAdjacentChunkPositiveNoAssert) {
		const glm::vec3 chunkPositive{CHUNK_LENGTH * 2, CHUNK_LENGTH * 2, CHUNK_LENGTH * 2};
		PackedBlockOffsetPosition packed = PackedBlockOffsetPosition::Pack(chunkPositive);
	}

	TEST(PackedBlockOffsetPositionTest, PackAndUnpackZero) {
		const glm::vec3 zero{0, 0, 0};
		PackedBlockOffsetPosition packed = PackedBlockOffsetPosition::Pack(zero);
		EXPECT_EQ(packed.Unpack(), zero);
	}

	TEST(PackedBlockOffsetPositionTest, PackAndUnpackNonZero) {
		const glm::vec3 pos{1, 2, 3};
		PackedBlockOffsetPosition packed = PackedBlockOffsetPosition::Pack(pos);
		EXPECT_EQ(packed.Unpack(), pos);
	}

	TEST(PackedBlockOffsetPositionTest, PackAndUnpackNegative) {
		const glm::vec3 pos{-1, -2, -3};
		PackedBlockOffsetPosition packed = PackedBlockOffsetPosition::Pack(pos);
		EXPECT_EQ(packed.Unpack(), pos);
	}

	TEST(PackedBlockOffsetPositionTest, PackAndUnpackLargerPositive) {
		const glm::vec3 pos{11, 12, 13};
		PackedBlockOffsetPosition packed = PackedBlockOffsetPosition::Pack(pos);
		EXPECT_EQ(packed.Unpack(), pos);
	}

	TEST(PackedBlockOffsetPositionTest, XComponentLosslessAtIntegers) {
		for (int i = (-1 * CHUNK_LENGTH); i <= (CHUNK_LENGTH * 2); i++) { // -8 -> 16 inclusive
			const glm::vec3 pos{i, 0, 0};
			PackedBlockOffsetPosition packed = PackedBlockOffsetPosition::Pack(pos);
			EXPECT_EQ(packed.Unpack().x, pos.x); 
			EXPECT_EQ(packed.Unpack(), pos);
		}
	}

	TEST(PackedBlockOffsetPositionTest, YComponentLosslessAtIntegers) {
		for (int i = (-1 * CHUNK_LENGTH); i <= (CHUNK_LENGTH * 2); i++) { // -8 -> 16 inclusive
			const glm::vec3 pos{0, i, 0};
			PackedBlockOffsetPosition packed = PackedBlockOffsetPosition::Pack(pos);
			EXPECT_EQ(packed.Unpack().y, pos.y);
			EXPECT_EQ(packed.Unpack(), pos);
		}
	}

	TEST(PackedBlockOffsetPositionTest, ZComponentLosslessAtIntegers) {
		for (int i = (-1 * CHUNK_LENGTH); i <= (CHUNK_LENGTH * 2); i++) { // -8 -> 16 inclusive
			const glm::vec3 pos{0, 0, i};
			PackedBlockOffsetPosition packed = PackedBlockOffsetPosition::Pack(pos);
			EXPECT_EQ(packed.Unpack().z, pos.z);
			EXPECT_EQ(packed.Unpack(), pos);
		}
	}

	TEST(PackedBlockOffsetPositionTest, Lossless16th) {
		const glm::vec3 pos{1.f / 16.f, 0, 0};
		PackedBlockOffsetPosition packed = PackedBlockOffsetPosition::Pack(pos);
		EXPECT_EQ(packed.Unpack().x, 1.f / 16.f);
		EXPECT_EQ(packed.Unpack(), pos);
	}

	TEST(PackedBlockOffsetPositionTest, RoundTo16th) {
		const glm::vec3 pos{3.f / 32.f, 0, 0}; // 1.5 / 16
		PackedBlockOffsetPosition packed = PackedBlockOffsetPosition::Pack(pos);
		EXPECT_EQ(packed.Unpack().x, 1.f / 16.f);
	}

	TEST(PackedBlockOffsetPositionTest, Lossless16thNegativeExtension) {
		const glm::vec3 pos{(1.f / 16.f) - 8, 0, 0};
		PackedBlockOffsetPosition packed = PackedBlockOffsetPosition::Pack(pos);
		EXPECT_EQ(packed.Unpack().x, (1.f / 16.f) - 8);
		EXPECT_EQ(packed.Unpack(), pos);
	}

	TEST(PackedBlockOffsetPositionTest, Lossless16thPositiveExtension) {
		const glm::vec3 pos{(1.f / 16.f) + 8, 0, 0};
		PackedBlockOffsetPosition packed = PackedBlockOffsetPosition::Pack(pos);
		EXPECT_EQ(packed.Unpack().x, (1.f / 16.f) + 8);
		EXPECT_EQ(packed.Unpack(), pos);
	}
}

#endif