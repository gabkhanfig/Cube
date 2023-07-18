#include "../../../CubeTest.h"
#include "../../../../Source/World/WorldTransform.h"

#if WITH_TESTS

namespace UnitTests
{
	TEST(ChunkPositionTest, Construct) {
		ChunkPosition pos;
		EXPECT_EQ(pos.x, 0);
		EXPECT_EQ(pos.y, 0);
		EXPECT_EQ(pos.z, 0);
	}

	TEST(ChunkPositionTest, ConstructValues) {
		ChunkPosition pos{ 1, -5, 200 };
		EXPECT_EQ(pos.x, 1);
		EXPECT_EQ(pos.y, -5);
		EXPECT_EQ(pos.z, 200);
	}

	TEST(ChunkPositionTest, CopyConstruct) {
		ChunkPosition pos{ 1, -5, 200 };
		ChunkPosition copy{ pos };
		EXPECT_EQ(copy.x, 1);
		EXPECT_EQ(copy.y, -5);
		EXPECT_EQ(copy.z, 200);
	}

	TEST(ChunkPositionTest, AssignmentCopy) {
		ChunkPosition pos{ 1, -5, 200 };
		ChunkPosition copy;
		copy = pos;
		EXPECT_EQ(copy.x, 1);
		EXPECT_EQ(copy.y, -5);
		EXPECT_EQ(copy.z, 200);
	}

	TEST(ChunkPositionTest, Equal) {
		ChunkPosition pos{ 1, -5, 200 };
		ChunkPosition copy{ 1, -5, 200 };
		EXPECT_EQ(pos, copy);
	}

	TEST(ChunkPositionTest, NotEqual) {
		ChunkPosition pos{ 1, -5, 200 };
		ChunkPosition copy{ 1, -6, 200 };
		EXPECT_NE(pos, copy);

	}
}

#endif