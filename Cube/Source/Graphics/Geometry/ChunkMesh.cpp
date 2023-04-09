#include "ChunkMesh.h"
#include "../OpenGL/Buffers/IndexBufferObject.h"
#include "../OpenGL/Buffers/VertexBufferObject.h"
#include "../../World/Chunk/ChunkData.h"
#include <chrono>

ChunkMesh::ChunkMesh()
{
  quads.Reserve(CHUNK_LENGTH * CHUNK_LENGTH * 6);
}

//VertexBufferObject* ChunkMesh::MakeVertexBufferObject() const
//{
//  return VertexBufferObject::Create<BlockQuad>(quads.Data(), quads.Size());
//}
//
//IndexBufferObject* ChunkMesh::MakeIndexBufferObject() const
//{
//  constexpr uint32 indexCount = 6;
//
//  // I have no idea why, but indices in this order just works for linear coordinates for quads. Do not change. 2, 3, 0, 1, 2, 0
//  constexpr uint32 indices[indexCount] = {
//    2, 3, 0, 1, 2, 0
//  };
//
//  const uint32 quadCount = quads.Size();
//
//  checkm((quadCount * indexCount) < (0x7FFFFFFF / sizeof(uint32)), "BlockQuad indices amount cannot exceed max signed 32-bit int / 4");
//
//  uint32 index = 0;
//  uint32* quadIndices = new uint32[uint64(indexCount) * uint64(quadCount)];
//  for (uint32 i = 0; i < quadCount; i++) {
//    quadIndices[index++] = (indices[0] + (i * 4));
//    quadIndices[index++] = (indices[1] + (i * 4));
//    quadIndices[index++] = (indices[2] + (i * 4));
//    quadIndices[index++] = (indices[3] + (i * 4));
//    quadIndices[index++] = (indices[4] + (i * 4));
//    quadIndices[index++] = (indices[5] + (i * 4));
//  }
//  IndexBufferObject* ibo = new IndexBufferObject(quadIndices, indexCount * quadCount);
//
//  delete[] quadIndices;
//  return ibo;
//}

void ChunkMesh::CopyQuadsToBuffer(BlockQuad* buffer) const
{
  checkm(buffer, "VBO BlockQuad buffer cannot be null");
  memcpy(buffer, quads.Data(), quads.Size() * sizeof(BlockQuad));
}

void ChunkMesh::CopyIndicesToBuffer(uint32* buffer, uint32 indexOffset) const
{
  checkm(buffer, "IBO quad indices buffer cannot be null");
  // I have no idea why, but indices in this order just works for linear coordinates for quads. Do not change. 2, 3, 0, 1, 2, 0
  constexpr uint32 indices[6] = {
    2, 3, 0, 1, 2, 0
  };
  const uint32 quadCount = quads.Size();
  uint32 index = 0; 
  for (uint32 i = 0; i < quadCount; i++) {
    buffer[index++] = (indices[0] + (i * 4) + indexOffset);
    buffer[index++] = (indices[1] + (i * 4) + indexOffset);
    buffer[index++] = (indices[2] + (i * 4) + indexOffset);
    buffer[index++] = (indices[3] + (i * 4) + indexOffset);
    buffer[index++] = (indices[4] + (i * 4) + indexOffset);
    buffer[index++] = (indices[5] + (i * 4) + indexOffset);
  }
}

void ChunkMesh::Empty()
{
  quads.Empty();
}

void ChunkMesh::AddQuad(const BlockQuad& quad)
{
  quads.Add(quad);
}

darray<uint32> ChunkMesh::GetIndices() const
{
  constexpr uint32 indices[6] = {
    2, 3, 0, 1, 2, 0
  };
  const uint32 quadCount = quads.Size();

  darray<uint32> outIndices;
  for (int i = 0; i < quadCount; i++) {
    outIndices.Add(indices[0] + (i * 4));
    outIndices.Add(indices[1] + (i * 4));
    outIndices.Add(indices[2] + (i * 4));
    outIndices.Add(indices[3] + (i * 4));
    outIndices.Add(indices[4] + (i * 4));
    outIndices.Add(indices[5] + (i * 4));
  }
  return outIndices;
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
