#include "../../../CubeTest.h"
#include "../../../../Source/World/WorldTransform.h"

#if WITH_TESTS

namespace UnitTests
{
	TEST(BlockPositionTest, Construct) {
		BlockPosition b;
		EXPECT_EQ(b.index, 0);
	}

	TEST(BlockPositionTest, ConstructSpecificIndex) {
		BlockPosition b{ 511 };
		EXPECT_EQ(b.index, 511);
	}

	TEST(BlockPositionTest, ConstructXYZ) {
		BlockPosition b{ 7, 7, 7 };
		EXPECT_EQ(b.index, 511);
	}

	TEST(BlockPositionTest, CopyConstruct) {
		BlockPosition b{ 7, 7, 7 };
		BlockPosition copy = b;
		EXPECT_EQ(b.index, 511);
	}

	TEST(BlockPositionTest, AssignIndex) {
		BlockPosition b;
		b = 5;
		EXPECT_EQ(b.index, 5);
	}

	TEST(BlockPositionTest, AssignCopy) {
		BlockPosition b{ 7, 7, 7 };
		BlockPosition copy;
		copy = b;
		EXPECT_EQ(b.index, 511);
	}

	TEST(BlockPositionTest, Equal) {
		BlockPosition b{ 4, 6, 0 };
		BlockPosition other{ 4, 6, 0 };
		EXPECT_EQ(b, other);
	}

	TEST(BlockPositionTest, NotEqual) {
		BlockPosition b{ 4, 6, 0 };
		BlockPosition other{ 4, 6, 1 };
		EXPECT_NE(b, other);
	}

	TEST(BlockPositionTest, DefaultGetXYZ) {
		BlockPosition b;
		EXPECT_EQ(b.X(), 0);
		EXPECT_EQ(b.Y(), 0);
		EXPECT_EQ(b.Z(), 0);
	}

	TEST(BlockPositionTest, GetXYZFromIndex) {
		BlockPosition b{ 201 };
		EXPECT_EQ(b.X(), 1);
		EXPECT_EQ(b.Y(), 3);
		EXPECT_EQ(b.Z(), 1);
	}

	TEST(BlockPositionTest, GetXYZFromXYZ) {
		BlockPosition b{ 4, 5, 2 };
		EXPECT_EQ(b.X(), 4);
		EXPECT_EQ(b.Y(), 5);
		EXPECT_EQ(b.Z(), 2);
	}
}

#endif