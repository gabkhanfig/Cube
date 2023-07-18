
#include "../../../CubeTest.h"
#include "../../../../Source/World/Chunk/BuriedChunkBlocks.h"

#if WITH_TESTS

namespace UnitTests 
{
	TEST(BuriedChunkBlocksTest, Construct) {
    BuriedChunkBlocks b;
		const uint64* arr = b.GetBitmaskAsIntArray();
		for (int i = 0; i < 8; i++) {
			EXPECT_EQ(arr[i], 0);
		}
	}

	TEST(BuriedChunkBlocksTest, ConstructIsBlockBuried) {
		BuriedChunkBlocks b;
		for (int i = 0; i < CHUNK_SIZE; i++) {
			ASSERT_FALSE(b.IsBlockBuried(i));
		}
	}

	TEST(BuriedChunkBlocksTest, SetFirstBlockBuried) {
		BuriedChunkBlocks b;
		b.SetBlockBuriedState(BlockPosition(0), true);
		EXPECT_TRUE(b.IsBlockBuried(BlockPosition(0)));
		EXPECT_FALSE(b.IsBlockBuried(BlockPosition(1)));
	}

	TEST(BuriedChunkBlocksTest, Set63rdBlockBuried) {
		BuriedChunkBlocks b;
		b.SetBlockBuriedState(BlockPosition(63), true);
		EXPECT_NE(b.GetBitmaskAsIntArray()[0], 0);
		EXPECT_FALSE(b.IsBlockBuried(BlockPosition(62)));
		EXPECT_TRUE(b.IsBlockBuried(BlockPosition(63)));
		EXPECT_FALSE(b.IsBlockBuried(BlockPosition(64)));
	}

	TEST(BuriedChunkBlocksTest, Set64thBlockBuried) {
		BuriedChunkBlocks b;
		b.SetBlockBuriedState(BlockPosition(64), true);
		EXPECT_EQ(b.GetBitmaskAsIntArray()[0], 0);
		EXPECT_TRUE(b.IsBlockBuried(BlockPosition(64)));
	}

	TEST(BuriedChunkBlocksTest, SetBlockToNotBuried) {
		BuriedChunkBlocks b;
		b.SetBlockBuriedState(BlockPosition(0), true);
		EXPECT_TRUE(b.IsBlockBuried(BlockPosition(0)));
		b.SetBlockBuriedState(BlockPosition(0), false);
		EXPECT_FALSE(b.IsBlockBuried(BlockPosition(0)));
	}

	TEST(BuriedChunkBlocksTest, Set63rdBlockToNotBuried) {
		BuriedChunkBlocks b;
		b.SetBlockBuriedState(BlockPosition(63), true);
		EXPECT_NE(b.GetBitmaskAsIntArray()[0], 0);
		EXPECT_FALSE(b.IsBlockBuried(BlockPosition(62)));
		EXPECT_TRUE(b.IsBlockBuried(BlockPosition(63)));
		EXPECT_FALSE(b.IsBlockBuried(BlockPosition(64)));
		b.SetBlockBuriedState(BlockPosition(63), false);
		EXPECT_EQ(b.GetBitmaskAsIntArray()[0], 0);
		EXPECT_FALSE(b.IsBlockBuried(BlockPosition(62)));
		EXPECT_FALSE(b.IsBlockBuried(BlockPosition(63)));
		EXPECT_FALSE(b.IsBlockBuried(BlockPosition(64)));

	}

	TEST(BuriedChunkBlocksTest, Set64thBlockToNotBuried) {
		BuriedChunkBlocks b;
		b.SetBlockBuriedState(BlockPosition(64), true);
		EXPECT_EQ(b.GetBitmaskAsIntArray()[0], 0);
		EXPECT_FALSE(b.IsBlockBuried(BlockPosition(63)));
		EXPECT_TRUE(b.IsBlockBuried(BlockPosition(64)));
		EXPECT_FALSE(b.IsBlockBuried(BlockPosition(65)));
		b.SetBlockBuriedState(BlockPosition(64), false);
		EXPECT_EQ(b.GetBitmaskAsIntArray()[0], 0);
		EXPECT_FALSE(b.IsBlockBuried(BlockPosition(63)));
		EXPECT_FALSE(b.IsBlockBuried(BlockPosition(64)));
		EXPECT_FALSE(b.IsBlockBuried(BlockPosition(65)));
	}

	TEST(BuriedChunkBlocksTest, Reset) {
		BuriedChunkBlocks b;
		b.SetBlockBuriedState(BlockPosition(0), true);
		EXPECT_TRUE(b.IsBlockBuried(BlockPosition(0)));
		b.Reset();
		EXPECT_FALSE(b.IsBlockBuried(BlockPosition(0)));
	}
	
	TEST(BuriedChunkBlocksTest, AreAllBlocksBuriedFalse) {
		BuriedChunkBlocks b;
		b.SetBlockBuriedState(BlockPosition(0), true);
		EXPECT_FALSE(b.AreAllBlocksBuried());
	}

	TEST(BuriedChunkBlocksTest, AreAllBlocksBuriedTrue) {
		BuriedChunkBlocks b;
		for (int i = 0; i < CHUNK_SIZE; i++) {
			b.SetBlockBuriedState(BlockPosition(i), true);
		}
		EXPECT_TRUE(b.AreAllBlocksBuried());
	}

	TEST(BuriedChunkBlocksTest, SetEqualToFirstBlockBuried) {
		BuriedChunkBlocks b;
		b.SetBlockBuriedState(BlockPosition(0), true);
		BuriedChunkBlocks other;
		other = b;
		EXPECT_TRUE(other.IsBlockBuried(BlockPosition(0)));
	}

	TEST(BuriedChunkBlocksTest, SetEqualToRandomBlocksBuried) {
		BuriedChunkBlocks b;
		b.SetBlockBuriedState(BlockPosition(2), true);
		b.SetBlockBuriedState(BlockPosition(67), true);
		b.SetBlockBuriedState(BlockPosition(200), true);
		b.SetBlockBuriedState(BlockPosition(511), true);
		BuriedChunkBlocks other;
		other = b;
		EXPECT_FALSE(other.IsBlockBuried(BlockPosition(0)));
		EXPECT_TRUE(other.IsBlockBuried(BlockPosition(2)));
		EXPECT_TRUE(other.IsBlockBuried(BlockPosition(67)));
		EXPECT_TRUE(other.IsBlockBuried(BlockPosition(200)));
		EXPECT_TRUE(other.IsBlockBuried(BlockPosition(511)));
	}

	TEST(BuriedChunkBlocksTest, CopyConstructor) {
		BuriedChunkBlocks b;
		b.SetBlockBuriedState(BlockPosition(2), true);
		b.SetBlockBuriedState(BlockPosition(67), true);
		b.SetBlockBuriedState(BlockPosition(200), true);
		b.SetBlockBuriedState(BlockPosition(511), true);
		BuriedChunkBlocks other = b;
		EXPECT_FALSE(other.IsBlockBuried(BlockPosition(0)));
		EXPECT_TRUE(other.IsBlockBuried(BlockPosition(2)));
		EXPECT_TRUE(other.IsBlockBuried(BlockPosition(67)));
		EXPECT_TRUE(other.IsBlockBuried(BlockPosition(200)));
		EXPECT_TRUE(other.IsBlockBuried(BlockPosition(511)));
	}

	TEST(BuriedChunkBlocksTest, EqualCopied) {
		BuriedChunkBlocks b;
		b.SetBlockBuriedState(BlockPosition(2), true);
		b.SetBlockBuriedState(BlockPosition(67), true);
		b.SetBlockBuriedState(BlockPosition(200), true);
		b.SetBlockBuriedState(BlockPosition(511), true);
		BuriedChunkBlocks other = b;
		EXPECT_EQ(b, other);
	}

	TEST(BuriedChunkBlocksTest, EqualManual) {
		BuriedChunkBlocks a;
		a.SetBlockBuriedState(BlockPosition(2), true);
		a.SetBlockBuriedState(BlockPosition(67), true);
		a.SetBlockBuriedState(BlockPosition(68), true);
		a.SetBlockBuriedState(BlockPosition(200), true);
		a.SetBlockBuriedState(BlockPosition(511), true);
		BuriedChunkBlocks b;
		b.SetBlockBuriedState(BlockPosition(2), true);
		b.SetBlockBuriedState(BlockPosition(67), true);
		b.SetBlockBuriedState(BlockPosition(68), true);
		b.SetBlockBuriedState(BlockPosition(200), true);
		b.SetBlockBuriedState(BlockPosition(511), true);
		EXPECT_EQ(a, b);
	}

	TEST(BuriedChunkBlocksTest, NotEqual) {
		BuriedChunkBlocks a;
		a.SetBlockBuriedState(BlockPosition(2), true);
		a.SetBlockBuriedState(BlockPosition(67), true);
		a.SetBlockBuriedState(BlockPosition(68), true);
		a.SetBlockBuriedState(BlockPosition(200), true);
		BuriedChunkBlocks b;
		b.SetBlockBuriedState(BlockPosition(2), true);
		b.SetBlockBuriedState(BlockPosition(67), true);
		b.SetBlockBuriedState(BlockPosition(68), true);
		b.SetBlockBuriedState(BlockPosition(200), true);
		b.SetBlockBuriedState(BlockPosition(511), true);
		EXPECT_NE(a, b);
	}
}

#endif