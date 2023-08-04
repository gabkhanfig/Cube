#include "../../../CubeTest.h"
#include "../../../../Source/World/WorldTransform.h"

#if WITH_TESTS

namespace UnitTests 
{
	TEST(BlockFacingTest, Construct) {
		BlockFacing b;
		EXPECT_EQ(b.facing, 0);
	}

	TEST(BlockFacingTest, ConstructWithDirection) {
		BlockFacing b = BlockFacing(BlockFacing::Dir_East | BlockFacing::Dir_Up);
		EXPECT_EQ(b.facing, BlockFacing::Dir_East | BlockFacing::Dir_Up);
	}

	TEST(BlockFacingTest, IsFacingDirection) {
		BlockFacing b{ BlockFacing::Dir_West | BlockFacing::Dir_South };
		EXPECT_TRUE(b.IsFacing(BlockFacing::Dir_West));
		EXPECT_TRUE(b.IsFacing(BlockFacing::Dir_South));
		EXPECT_FALSE(b.IsFacing(BlockFacing::Dir_Down));
	}

	TEST(BlockFacingTest, CopyConstructor) {
		BlockFacing b{ BlockFacing::Dir_West | BlockFacing::Dir_South };
		BlockFacing copy = b;
		EXPECT_TRUE(copy.IsFacing(BlockFacing::Dir_West));
		EXPECT_TRUE(copy.IsFacing(BlockFacing::Dir_South));
		EXPECT_FALSE(copy.IsFacing(BlockFacing::Dir_Down));
	}

	TEST(BlockFacingTest, AssignmentOperatorBitmask) {
		BlockFacing b;
		EXPECT_EQ(b.facing, BlockFacing::Dir_Down);
		b = BlockFacing::Dir_North | BlockFacing::Dir_East;
		EXPECT_TRUE(b.IsFacing(BlockFacing::Dir_North));
		EXPECT_TRUE(b.IsFacing(BlockFacing::Dir_East));
		EXPECT_FALSE(b.IsFacing(BlockFacing::Dir_Down));
	}

	TEST(BlockFacingTest, AssignmentOperatorOther) {
		BlockFacing b;
		EXPECT_EQ(b.facing, BlockFacing::Dir_Down);
		BlockFacing other = BlockFacing::Dir_North | BlockFacing::Dir_East;
		b = other;
		EXPECT_TRUE(b.IsFacing(BlockFacing::Dir_North));
		EXPECT_TRUE(b.IsFacing(BlockFacing::Dir_East));
		EXPECT_FALSE(b.IsFacing(BlockFacing::Dir_Down));
	}

	TEST(BlockFacingTest, Opposite) {
		BlockFacing b = BlockFacing::Dir_East | BlockFacing::Dir_Up;
		BlockFacing opposite = b.Opposite();
		EXPECT_TRUE(opposite.IsFacing(BlockFacing::Dir_Down));
		EXPECT_TRUE(opposite.IsFacing(BlockFacing::Dir_West));
		EXPECT_FALSE(opposite.IsFacing(BlockFacing::Dir_East));
		EXPECT_FALSE(opposite.IsFacing(BlockFacing::Dir_Up));
	}

}

#endif