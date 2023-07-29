#include "../../../CubeTest.h"
#include "../../../../Source/Graphics/Geometry/PackedTypes.h"

#if WITH_TESTS

namespace UnitTests
{
	TEST(PackedNormalTest, PackLengthOneNoAssert) {
		const glm::vec3 vec{1, 0, 0};
		PackedNormal normal = PackedNormal::Pack(vec);
	}

	TEST(PackedNormalTest, PackAndUnpackX) {
		const glm::vec3 vec{1, 0, 0};
		PackedNormal normal = PackedNormal::Pack(vec);
		EXPECT_EQ(normal.Unpack(), vec);
	}
	
	TEST(PackedNormalTest, PackAndUnpackY) {
		const glm::vec3 vec{0, 1, 0};
		PackedNormal normal = PackedNormal::Pack(vec);
		EXPECT_EQ(normal.Unpack(), vec);
	}
	
	TEST(PackedNormalTest, PackAndUnpackZ) {
		const glm::vec3 vec{0, 0, 1};
		PackedNormal normal = PackedNormal::Pack(vec);
		EXPECT_EQ(normal.Unpack(), vec);
	}

	TEST(PackedNormalTest, NegativePackAndUnpackX) {
		const glm::vec3 vec{-1, 0, 0};
		PackedNormal normal = PackedNormal::Pack(vec);
		EXPECT_EQ(normal.Unpack(), vec);
	}

	TEST(PackedNormalTest, NegativePackAndUnpackY) {
		const glm::vec3 vec{0, -1, 0};
		PackedNormal normal = PackedNormal::Pack(vec);
		EXPECT_EQ(normal.Unpack(), vec);
	}

	TEST(PackedNormalTest, NegativePackAndUnpackZ) {
		const glm::vec3 vec{0, 0, -1};
		PackedNormal normal = PackedNormal::Pack(vec);
		EXPECT_EQ(normal.Unpack(), vec);
	}

	TEST(PackedNormalTest, AccurateAtPositive1) {
		const glm::vec3 vec{1, 0, 0};
		PackedNormal normal = PackedNormal::Pack(vec);
		EXPECT_EQ(normal.Unpack().x, 1.f);
	}

	TEST(PackedNormalTest, AccurateAtZero) {
		const glm::vec3 vec{0, 0, 1};
		PackedNormal normal = PackedNormal::Pack(vec);
		EXPECT_EQ(normal.Unpack().x, 0.f);
	}

	TEST(PackedNormalTest, AccurateAtNegative1) {
		const glm::vec3 vec{-1, 0, 0};
		PackedNormal normal = PackedNormal::Pack(vec);
		EXPECT_EQ(normal.Unpack().x, -1.f);
	}

	TEST(PackedNormalTest, MixedValues) {
		const glm::vec3 vec{1.f/2.f, 0, sqrt(3.f)/2.f};
		PackedNormal normal = PackedNormal::Pack(vec);
		const glm::vec3 unpacked = normal.Unpack();
		EXPECT_TRUE(IsNearlyEqual(unpacked.x, vec.x, 0.002f));
		EXPECT_TRUE(IsNearlyEqual(unpacked.y, vec.y, 0.002f));
		EXPECT_TRUE(IsNearlyEqual(unpacked.z, vec.z, 0.002f));
	}
}

#endif