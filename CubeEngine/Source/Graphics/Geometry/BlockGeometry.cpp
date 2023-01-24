#include "BlockGeometry.h"
#include "../Buffers/IndexBufferObject.h"
#include "../Buffers/VertexBufferObject.h"

IndexBufferObject* BlockQuad::CreateQuadsIndexBuffer(const uint32 quadCount)
{
  constexpr uint32 indexCount = 6;
  constexpr uint32 indices[indexCount] = {
    0, 1, 2,
    3, 0, 2
  };

  uint32 index = 0;
  uint32* quadIndices = new uint32[indexCount * quadCount];
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
  layout.Push<uint32>(1); // position
  layout.Push<float>(2);  // texCoord
  return layout;
}

const VertexBufferLayout& BlockQuad::GetQuadsVertexBufferLayout()
{
  static VertexBufferLayout layout = QuadVBL();
  return layout;
}
