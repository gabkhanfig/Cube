
#include "../../../CubeTest.h"
#include "../../../../Source/World/Chunk/ChunkBlocksBitmask.h"

#if WITH_TESTS

namespace UnitTests 
{
	TEST(ChunkBlocksBitmaskTest, Construct) {
    ChunkBlocksBitmask b;
		const uint64* arr = b.GetBitmaskAsIntArray();
		for (int i = 0; i < 8; i++) {
			EXPECT_EQ(arr[i], 0);
		}
	}

	TEST(ChunkBlocksBitmaskTest, ConstructGetFlag) {
		ChunkBlocksBitmask b;
		for (int i = 0; i < CHUNK_SIZE; i++) {
			ASSERT_FALSE(b.GetFlag(i));
		}
	}

	TEST(ChunkBlocksBitmaskTest, SetFirstBlockSet) {
		ChunkBlocksBitmask b;
		b.SetFlag(BlockPosition(0), true);
		EXPECT_TRUE(b.GetFlag(BlockPosition(0)));
		EXPECT_FALSE(b.GetFlag(BlockPosition(1)));
	}

	TEST(ChunkBlocksBitmaskTest, Set63rdBlockSet) {
		ChunkBlocksBitmask b;
		b.SetFlag(BlockPosition(63), true);
		EXPECT_NE(b.GetBitmaskAsIntArray()[0], 0);
		EXPECT_FALSE(b.GetFlag(BlockPosition(62)));
		EXPECT_TRUE(b.GetFlag(BlockPosition(63)));
		EXPECT_FALSE(b.GetFlag(BlockPosition(64)));
	}

	TEST(ChunkBlocksBitmaskTest, Set64thBlockSet) {
		ChunkBlocksBitmask b;
		b.SetFlag(BlockPosition(64), true);
		EXPECT_EQ(b.GetBitmaskAsIntArray()[0], 0);
		EXPECT_TRUE(b.GetFlag(BlockPosition(64)));
	}

	TEST(ChunkBlocksBitmaskTest, SetBlockToNotSet) {
		ChunkBlocksBitmask b;
		b.SetFlag(BlockPosition(0), true);
		EXPECT_TRUE(b.GetFlag(BlockPosition(0)));
		b.SetFlag(BlockPosition(0), false);
		EXPECT_FALSE(b.GetFlag(BlockPosition(0)));
	}

	TEST(ChunkBlocksBitmaskTest, Set63rdBlockToNotSet) {
		ChunkBlocksBitmask b;
		b.SetFlag(BlockPosition(63), true);
		EXPECT_NE(b.GetBitmaskAsIntArray()[0], 0);
		EXPECT_FALSE(b.GetFlag(BlockPosition(62)));
		EXPECT_TRUE(b.GetFlag(BlockPosition(63)));
		EXPECT_FALSE(b.GetFlag(BlockPosition(64)));
		b.SetFlag(BlockPosition(63), false);
		EXPECT_EQ(b.GetBitmaskAsIntArray()[0], 0);
		EXPECT_FALSE(b.GetFlag(BlockPosition(62)));
		EXPECT_FALSE(b.GetFlag(BlockPosition(63)));
		EXPECT_FALSE(b.GetFlag(BlockPosition(64)));

	}

	TEST(ChunkBlocksBitmaskTest, Set64thBlockToNotSet) {
		ChunkBlocksBitmask b;
		b.SetFlag(BlockPosition(64), true);
		EXPECT_EQ(b.GetBitmaskAsIntArray()[0], 0);
		EXPECT_FALSE(b.GetFlag(BlockPosition(63)));
		EXPECT_TRUE(b.GetFlag(BlockPosition(64)));
		EXPECT_FALSE(b.GetFlag(BlockPosition(65)));
		b.SetFlag(BlockPosition(64), false);
		EXPECT_EQ(b.GetBitmaskAsIntArray()[0], 0);
		EXPECT_FALSE(b.GetFlag(BlockPosition(63)));
		EXPECT_FALSE(b.GetFlag(BlockPosition(64)));
		EXPECT_FALSE(b.GetFlag(BlockPosition(65)));
	}

	TEST(ChunkBlocksBitmaskTest, Reset) {
		ChunkBlocksBitmask b;
		b.SetFlag(BlockPosition(0), true);
		EXPECT_TRUE(b.GetFlag(BlockPosition(0)));
		b.Reset();
		EXPECT_FALSE(b.GetFlag(BlockPosition(0)));
	}
	
	TEST(ChunkBlocksBitmaskTest, AreAllBlocksSetFalse) {
		ChunkBlocksBitmask b;
		b.SetFlag(BlockPosition(0), true);
		EXPECT_FALSE(b.AreAllBlocksSet());
	}

	TEST(ChunkBlocksBitmaskTest, AreAllBlocksSetTrue) {
		ChunkBlocksBitmask b;
		for (int i = 0; i < CHUNK_SIZE; i++) {
			b.SetFlag(BlockPosition(i), true);
		}
		EXPECT_TRUE(b.AreAllBlocksSet());
	}

	TEST(ChunkBlocksBitmaskTest, SetEqualToFirstBlockSet) {
		ChunkBlocksBitmask b;
		b.SetFlag(BlockPosition(0), true);
		ChunkBlocksBitmask other;
		other = b;
		EXPECT_TRUE(other.GetFlag(BlockPosition(0)));
	}

	TEST(ChunkBlocksBitmaskTest, SetEqualToRandomBlocksSet) {
		ChunkBlocksBitmask b;
		b.SetFlag(BlockPosition(2), true);
		b.SetFlag(BlockPosition(67), true);
		b.SetFlag(BlockPosition(200), true);
		b.SetFlag(BlockPosition(511), true);
		ChunkBlocksBitmask other;
		other = b;
		EXPECT_FALSE(other.GetFlag(BlockPosition(0)));
		EXPECT_TRUE(other.GetFlag(BlockPosition(2)));
		EXPECT_TRUE(other.GetFlag(BlockPosition(67)));
		EXPECT_TRUE(other.GetFlag(BlockPosition(200)));
		EXPECT_TRUE(other.GetFlag(BlockPosition(511)));
	}

	TEST(ChunkBlocksBitmaskTest, CopyConstructor) {
		ChunkBlocksBitmask b;
		b.SetFlag(BlockPosition(2), true);
		b.SetFlag(BlockPosition(67), true);
		b.SetFlag(BlockPosition(200), true);
		b.SetFlag(BlockPosition(511), true);
		ChunkBlocksBitmask other = b;
		EXPECT_FALSE(other.GetFlag(BlockPosition(0)));
		EXPECT_TRUE(other.GetFlag(BlockPosition(2)));
		EXPECT_TRUE(other.GetFlag(BlockPosition(67)));
		EXPECT_TRUE(other.GetFlag(BlockPosition(200)));
		EXPECT_TRUE(other.GetFlag(BlockPosition(511)));
	}

	TEST(ChunkBlocksBitmaskTest, EqualCopied) {
		ChunkBlocksBitmask b;
		b.SetFlag(BlockPosition(2), true);
		b.SetFlag(BlockPosition(67), true);
		b.SetFlag(BlockPosition(200), true);
		b.SetFlag(BlockPosition(511), true);
		ChunkBlocksBitmask other = b;
		EXPECT_EQ(b, other);
	}

	TEST(ChunkBlocksBitmaskTest, EqualManual) {
		ChunkBlocksBitmask a;
		a.SetFlag(BlockPosition(2), true);
		a.SetFlag(BlockPosition(67), true);
		a.SetFlag(BlockPosition(68), true);
		a.SetFlag(BlockPosition(200), true);
		a.SetFlag(BlockPosition(511), true);
		ChunkBlocksBitmask b;
		b.SetFlag(BlockPosition(2), true);
		b.SetFlag(BlockPosition(67), true);
		b.SetFlag(BlockPosition(68), true);
		b.SetFlag(BlockPosition(200), true);
		b.SetFlag(BlockPosition(511), true);
		EXPECT_EQ(a, b);
	}

	TEST(ChunkBlocksBitmaskTest, NotEqual) {
		ChunkBlocksBitmask a;
		a.SetFlag(BlockPosition(2), true);
		a.SetFlag(BlockPosition(67), true);
		a.SetFlag(BlockPosition(68), true);
		a.SetFlag(BlockPosition(200), true);
		ChunkBlocksBitmask b;
		b.SetFlag(BlockPosition(2), true);
		b.SetFlag(BlockPosition(67), true);
		b.SetFlag(BlockPosition(68), true);
		b.SetFlag(BlockPosition(200), true);
		b.SetFlag(BlockPosition(511), true);
		EXPECT_NE(a, b);
	}

	TEST(ChunkBlocksBitmaskTest, FirstSetBlockIndex) {
		ChunkBlocksBitmask a;
		a.SetFlag(BlockPosition(5), true);
		EXPECT_EQ(a.FirstSetBlockIndex().Get(), 5);

		ChunkBlocksBitmask b;
		b.SetFlag(BlockPosition(67), true);
		EXPECT_EQ(b.FirstSetBlockIndex().Get(), 67);

		ChunkBlocksBitmask c;
		c.SetFlag(BlockPosition(300), true);
		EXPECT_EQ(c.FirstSetBlockIndex().Get(), 300);

		ChunkBlocksBitmask d;
		EXPECT_FALSE(d.FirstSetBlockIndex().IsValidIndex());
	}

	TEST(ChunkBlocksBitmaskTest, SetAllFlags) {
		ChunkBlocksBitmask a;
		a.SetAllFlags();
		EXPECT_TRUE(a.AreAllBlocksSet());
	}
}

#endif