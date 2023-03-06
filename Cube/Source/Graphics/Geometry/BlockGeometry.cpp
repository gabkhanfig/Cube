#include "BlockGeometry.h"
#include "../OpenGL/Buffers/IndexBufferObject.h"
#include "../OpenGL/Buffers/VertexBufferObject.h"

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
  layout.Push<float>(3);  // normal
  layout.Push<float>(2);  // texCoord
  layout.Push<float>(3);  // color
  return layout;
}

const VertexBufferLayout& BlockQuad::GetQuadsVertexBufferLayout()
{
  static VertexBufferLayout layout = QuadVBL();
  return layout;
}

const glm::vec3 BlockQuad::NormalFromQuadPoints(const glm::vec3 points[4])
{
  const glm::vec3 AB = points[1] - points[0];
  const glm::vec3 BC = points[2] - points[1];

#ifdef DEVELOPMENT

  const glm::vec3 CD = points[3] - points[2];
  const glm::vec3 DA = points[0] - points[3];

  constexpr float threshold = 0.01;
  
  const glm::vec3 ABcrossCD = glm::abs(glm::cross(AB, CD));
  const glm::vec3 BCcrossDA = glm::abs(glm::cross(BC, DA));
  //std::cout << "Cross of AB and CD = x: " << ABcrossCD.x << " y: " << ABcrossCD.y << " z: " << ABcrossCD.z << std::endl;
  //std::cout << "Cross of BC and DA = x: " << BCcrossDA.x << " y: " << BCcrossDA.y << " z: " << BCcrossDA.z << std::endl;
  checkm(ABcrossCD.x < threshold && ABcrossCD.y < threshold && ABcrossCD.z < threshold, "Block quad points must all lie on the same plane");
  checkm(BCcrossDA.x < threshold && BCcrossDA.y < threshold && BCcrossDA.z < threshold, "Block quad points must all lie on the same plane");

#endif

  return glm::cross(AB, BC);
}
