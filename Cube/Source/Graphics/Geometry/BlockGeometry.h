#pragma once

#include "../../Core/EngineTypes.h"
#include "../OpenGL/Buffers/VertexBufferLayout.h"
#include "PackedTypes.h"

/* Block vertex data to be sent to the GPU vertex shader. */
struct BlockVertex
{
  /* Relative chunk position of the block vertex. */
  PackedBlockOffsetPosition position;

  /* Normalized (-1 -> 1) normal-vector of this vertex. */
  PackedNormal normal;

  /* Texture coordinates of this vertex. See BlockTextureAtlas::GetTextureCoord(). */
  glm::vec2 texCoord;

  /**/
  PackedColor color;

  BlockVertex() = default;

  /* Requires an index buffer of { 0, 1, 2, 3, 0, 2 } for the color interpolation to work correctly.
  @param _positions: vertex position
  @param _colors: Bitmasks for the RGB colors of the four corners in the quad. */
  BlockVertex(PackedBlockOffsetPosition _position, PackedNormal _normal, glm::vec2 _texCoord, PackedColor _color)
    : position(_position), normal(_normal), texCoord(_texCoord), color(_color)
  {}
};
static_assert(sizeof(BlockVertex) ==
  sizeof(PackedBlockOffsetPosition) + sizeof(PackedNormal) + sizeof(glm::vec2) + sizeof(PackedColor)
  , "Block Vertex data is not tightly packed");

/* Block quad vertex position ordering goes top right, top left, bottom left, bottom right. Bottom left is relative (0, 0) */
struct BlockQuad
{
  BlockVertex vertices[4];

  BlockQuad() = default;

  /* Quad of 4 vertices. Calculates the normal given the 4 positions. */
  BlockQuad(const PackedBlockOffsetPosition positions[4], const glm::vec2 texCoords[4], const PackedColor colors[4])
  {
    const glm::vec3 vec3Positions[4] = {
      positions[0].Unpack(),
      positions[1].Unpack(),
      positions[2].Unpack(),
      positions[3].Unpack()
    };
    const PackedNormal normal = PackedNormal::Pack(NormalFromQuadPoints(vec3Positions));

    vertices[0] = BlockVertex(positions[0], normal, texCoords[0], colors[0]);
    vertices[1] = BlockVertex(positions[1], normal, texCoords[1], colors[1]);
    vertices[2] = BlockVertex(positions[2], normal, texCoords[2], colors[2]);
    vertices[3] = BlockVertex(positions[3], normal, texCoords[3], colors[3]);
  }

  static class IndexBufferObject* CreateQuadsIndexBuffer(const uint32 quadCount);

  static class VertexBufferObject* CreateQuadsVertexBufferObject(const BlockQuad* quads, const uint32 quadCount);

  /* Get the global vertex buffer layout for quads. */
  static const VertexBufferLayout& GetQuadsVertexBufferLayout();

  static const glm::vec3 NormalFromQuadPoints(const glm::vec3 points[4]);

};
static_assert(sizeof(BlockQuad) == sizeof(BlockVertex) * 4, "Block quad byte size must be equal to 4x BlockVertex");
