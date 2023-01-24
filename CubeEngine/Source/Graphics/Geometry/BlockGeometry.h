#pragma once

#include "../../Core/EngineTypes.h"
#include "../Buffers/VertexBufferLayout.h"

/* Block vertex data to be sent to the GPU vertex shader. */
struct BlockVertex
{
  /* Positions and direction. */
  uint32 position;
  glm::vec2 texCoord;

  BlockVertex() = default;

  /* Requires an index buffer of { 0, 1, 2, 3, 0, 2 } for the color interpolation to work correctly.
  @param _positions: vertex position
  @param _colors: Bitmasks for the RGB colors of the four corners in the quad. */
  BlockVertex(const uint32 _position, glm::vec2 _texCoord)
    : position(_position), texCoord(_texCoord)
  {}
};
static_assert(sizeof(BlockVertex) == 
  sizeof(uint32) + sizeof(glm::vec2)
  , "Block Vertex data is not tightly packed");

/* Block quad vertex position ordering goes top right, top left, bottom left, bottom right. Bottom left is relative (0, 0) */
struct BlockQuad
{
  BlockVertex vertices[4];

  
  BlockQuad(const uint32 positions[4], const glm::vec2 texCoords[4])
  {
    vertices[0] = BlockVertex(positions[0], texCoords[0]);
    vertices[1] = BlockVertex(positions[1], texCoords[1]);
    vertices[2] = BlockVertex(positions[2], texCoords[2]);
    vertices[3] = BlockVertex(positions[3], texCoords[3]);
  }

  static class IndexBufferObject* CreateQuadsIndexBuffer(const uint32 quadCount);

  static class VertexBufferObject* CreateQuadsVertexBufferObject(const BlockQuad* quads, const uint32 quadCount);

  /* Get the global vertex buffer layout for quads. */
  static const VertexBufferLayout& GetQuadsVertexBufferLayout();

};
static_assert(sizeof(BlockQuad) == sizeof(BlockVertex) * 4, "Block quad byte size must be equal to 4x BlockVertex");