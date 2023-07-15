#include "CubeTest.h"
#include "../Cube/Source/Engine/EngineCore.h"
#include "../Cube/Source/World/Chunk/BuriedChunkBlocks.h"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

int main() {
  const uint64 allBitsSet[4] = { ~0 };
  const __m256i setBitmask = _mm256_loadu_epi64(allBitsSet);
  std::cout << (int)_mm256_cmpeq_epi64_mask(setBitmask, setBitmask) << std::endl;
  Sleep(10000);
}