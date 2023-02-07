#include "BlockGeometry.h"
#include <Graphics/Buffers/IndexBufferObject.h>
#include <Graphics/Buffers/VertexBufferObject.h>

IndexBufferObject* BlockQuad::CreateQuadsIndexBuffer(const uint32 quadCount)
{
  constexpr uint32 indexCount = 6;

  // I have no idea why, but indices in this order just works for linear coordinates for quads. Do not change. 2, 3, 0, 1, 2, 0
  constexpr uint32 indices[indexCount] = {
    2, 3, 0, 1, 2, 0
  };

  checkm((quadCount * indexCount) < (0x7FFFFFFF / sizeof(uint32)), "BlockQuad indices amount cannot exceed max signed 32-bit int / 4");

  uint32 index = 0;
  uint32* quadIndices = new uint32[uint64(indexCount) * uint64(quadCount)];
  for (int i = 0; i < quadCount; i++) {
    quadIndices[index++] = (indices[0] + (i * 4));
    quadIndices[index++] = (indices[1] + (i * 4));
    quadIndices[index++] = (indices[2] + (i * 4));
    quadIndices[index++] = (indices[3] + (i * 4));
    quadIndices[index++] = (indices[4] + (i * 4));
    quadIndices[index++] = (indices[5] + (i * 4));
  }
  IndexBufferObject* ibo = new IndexBufferObject(quadIndices, indexCount * quadCount);
  delete[] quadIndices;
  return ibo;
}

VertexBufferObject* BlockQuad::CreateQuadsVertexBufferObject(const BlockQuad* quads, const uint32 quadCount)
{
  return VertexBufferObject::Create<BlockQuad>(quads, quadCount);
}

static VertexBufferLayout QuadVBL() {
  VertexBufferLayout layout;
  layout.Push<float>(3);  // position
  layout.Push<float>(2);  // texCoord
  return layout;
}

const VertexBufferLayout& BlockQuad::GetQuadsVertexBufferLayout()
{
  static VertexBufferLayout layout = QuadVBL();
  return layout;
}
