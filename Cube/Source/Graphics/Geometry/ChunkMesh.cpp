#include "ChunkMesh.h"
#include "../OpenGL/Buffers/IndexBufferObject.h"
#include "../OpenGL/Buffers/VertexBufferObject.h"
#include "../../World/Chunk/ChunkData.h"
#include <chrono>

ChunkMesh::ChunkMesh()
{
  quads.Reserve(CHUNK_LENGTH * CHUNK_LENGTH * 6);
}

VertexBufferObject* ChunkMesh::MakeVertexBufferObject() const
{
  return VertexBufferObject::Create<BlockQuad>(quads.Data(), quads.Size());
}

IndexBufferObject* ChunkMesh::MakeIndexBufferObject() const
{
  auto start1 = std::chrono::high_resolution_clock::now();
  
  constexpr uint32 indexCount = 6;

  // I have no idea why, but indices in this order just works for linear coordinates for quads. Do not change. 2, 3, 0, 1, 2, 0
  constexpr uint32 indices[indexCount] = {
    2, 3, 0, 1, 2, 0
  };

  const uint32 quadCount = quads.Size();

  checkm((quadCount * indexCount) < (0x7FFFFFFF / sizeof(uint32)), "BlockQuad indices amount cannot exceed max signed 32-bit int / 4");

  uint32 index = 0;
  uint32* quadIndices = new uint32[uint64(indexCount) * uint64(quadCount)];
  for (uint32 i = 0; i < quadCount; i++) {
    quadIndices[index++] = (indices[0] + (i * 4));
    quadIndices[index++] = (indices[1] + (i * 4));
    quadIndices[index++] = (indices[2] + (i * 4));
    quadIndices[index++] = (indices[3] + (i * 4));
    quadIndices[index++] = (indices[4] + (i * 4));
    quadIndices[index++] = (indices[5] + (i * 4));
  }
  auto stop1 = std::chrono::high_resolution_clock::now();
  auto start2 = std::chrono::high_resolution_clock::now();
  IndexBufferObject* ibo = new IndexBufferObject(quadIndices, indexCount * quadCount);
  auto stop2 = std::chrono::high_resolution_clock::now();

  std::cout << "Time to create IBO data:  " << std::chrono::duration_cast<std::chrono::microseconds>(stop1 - start1).count() << "us\n";
  std::cout << "Time to create GPU IBO: " << std::chrono::duration_cast<std::chrono::microseconds>(stop2 - start2).count() << "us\n";

  delete[] quadIndices;
  return ibo;
}

void ChunkMesh::Empty()
{
  quads.Empty();
}

void ChunkMesh::AddQuad(const BlockQuad& quad)
{
  quads.Add(quad);
}

uint32* ChunkMesh::CreateQuadIndices(const uint32 quadCount)
{
  constexpr uint32 indexCount = 6;

  // I have no idea why, but indices in this order just works for linear coordinates for quads. Do not change. 2, 3, 0, 1, 2, 0
  constexpr uint32 indices[indexCount] = {
    2, 3, 0, 1, 2, 0
  };

  checkm((quadCount * indexCount) < (0x7FFFFFFF / sizeof(uint32)), "BlockQuad indices amount cannot exceed max signed 32-bit int / 4");

  uint32 index = 0;
  uint32* quadIndices = new uint32[uint64(indexCount) * uint64(quadCount)];
  for (uint32 i = 0; i < quadCount; i++) {
    quadIndices[index++] = (indices[0] + (i * 4));
    quadIndices[index++] = (indices[1] + (i * 4));
    quadIndices[index++] = (indices[2] + (i * 4));
    quadIndices[index++] = (indices[3] + (i * 4));
    quadIndices[index++] = (indices[4] + (i * 4));
    quadIndices[index++] = (indices[5] + (i * 4));
  }
  return quadIndices;
}
