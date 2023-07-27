#include "../../../CubeTest.h"
#include "../../../../Source/World/WorldTransform.h"

#if WITH_TESTS

namespace UnitTests
{
	TEST(WorldPositionTest, Construct) {
		WorldPosition pos;
		EXPECT_EQ(pos.x, 0);
		EXPECT_EQ(pos.y, 0);
		EXPECT_EQ(pos.z, 0);
	}

	TEST(WorldPositionTest, ConstructXYZ) {
		WorldPosition pos = WorldPosition(20, -7, -600);
		EXPECT_EQ(pos.x, 20);
		EXPECT_EQ(pos.y, -7);
		EXPECT_EQ(pos.z, -600);
	}

	TEST(WorldPositionTest, CopyConstruct) {
		WorldPosition pos = WorldPosition(20, -7, -600);
		WorldPosition copy = pos;
		EXPECT_EQ(copy.x, 20);
		EXPECT_EQ(copy.y, -7);
		EXPECT_EQ(copy.z, -600);
	}

	TEST(WorldPositionTest, ConstructFromDVec3) {
		const glm::dvec3 vec{0.5, -11.1, 234978263.4857928347};
		WorldPosition pos = WorldPosition(vec);
		EXPECT_EQ(pos.x, 0);
		EXPECT_EQ(pos.y, -11);
		EXPECT_EQ(pos.z, 234978263);
	}

	TEST(WorldPositionTest, ConstructFromChunkPositionAndBlockPosition) {
		ChunkPosition cpos{ 0, 1, -1 };
		BlockPosition bpos{ 1, 5, 3 };
		WorldPosition pos = WorldPosition(cpos, bpos);
		EXPECT_EQ(pos.x, 1);
		EXPECT_EQ(pos.y, 13);
		EXPECT_EQ(pos.z, -5);
	}

	TEST(WorldPositionTest, AssignmentCopy) {
		WorldPosition pos = WorldPosition(20, -7, -600);
		WorldPosition copy;
		copy = pos;
		EXPECT_EQ(copy.x, 20);
		EXPECT_EQ(copy.y, -7);
		EXPECT_EQ(copy.z, -600);
	}

	TEST(WorldPositionTest, Equal) {
		WorldPosition pos = WorldPosition(20, -7, -600);
		WorldPosition other = WorldPosition(20, -7, -600);
		EXPECT_EQ(pos, other);
	}

	TEST(WorldPositionTest, NotEqual) {
		WorldPosition pos = WorldPosition(20, -7, -600);
		WorldPosition other = WorldPosition(20, -7, -600);
		EXPECT_EQ(pos, other);
	}

	TEST(WorldPositionTest, ToChunkPosition) {
		WorldPosition pos = WorldPosition(0, 7, -1);
		ChunkPosition cpos = pos.ToChunkPosition();
		EXPECT_EQ(cpos.x, 0);
		EXPECT_EQ(cpos.y, 0);
		EXPECT_EQ(cpos.z, -1);
	}

	TEST(WorldPositionTest, ToChunkPositionLargeValues) {
		WorldPosition pos = WorldPosition(1000, INT32_MAX, INT32_MIN);
		ChunkPosition cpos = pos.ToChunkPosition();
		EXPECT_EQ(cpos.x, 125);
		EXPECT_EQ(cpos.y, 268435455);
		EXPECT_EQ(cpos.z, -268435456);
	}

	TEST(WorldPositionTest, ToBlockPosition) {
		WorldPosition pos = WorldPosition(-1, 1, 0);
		BlockPosition bpos = pos.ToBlockPosition();
		EXPECT_EQ(bpos.X(), 7);
		EXPECT_EQ(bpos.Y(), 1);
		EXPECT_EQ(bpos.Z(), 0);
	}

	TEST(WorldPositionTest, ToChunkPositionAndToBlockPositionSanity) {
		WorldPosition pos = WorldPosition(-9, -8, -7);
		ChunkPosition cpos = pos.ToChunkPosition();
		BlockPosition bpos = pos.ToBlockPosition();

		EXPECT_EQ(bpos.X(), 7);
		EXPECT_EQ(bpos.Y(), 0);
		EXPECT_EQ(bpos.Z(), 1);

		EXPECT_EQ(cpos.x, -2);
		EXPECT_EQ(cpos.y, -1);
		EXPECT_EQ(cpos.z, -1);

	}
}

#endif