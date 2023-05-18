#pragma once

#include "../../Core/EngineTypes.h"
#include "../OpenGL/Buffers/VertexBufferLayout.h"

/* Block vertex data to be sent to the GPU vertex shader. */
struct BlockVertex
{
  /* Compressed normalized vertex normals with a mapped range of float -1 = uint8 0, float 1 = uint8 254. To be unpacked in gpu. */
  struct PackedNormal
  {
    uint8 x; // When cast to 4 byte int, first byte (num & 255) is x
    uint8 y; // When cast to 4 byte int, second byte (num >> 8 & 255) is y
    uint8 z; // When cast to 4 byte int, third byte (num >> 16 & 255) is z
    uint8 _padding;

    static PackedNormal FromVec3(const glm::vec3 normal) {
      PackedNormal pnorm;
      pnorm.x = normal.x * 127 + 127;
      pnorm.y = normal.y * 127 + 127;
      pnorm.z = normal.z * 127 + 127;
    }
  };

  struct PackedColor
  {
    uint8 r; // When cast to 4 byte int, first byte (num & 255) is r
    uint8 b; // When cast to 4 byte int, second byte (num >> 8 & 255) is g
    uint8 g; // When cast to 4 byte int, third byte (num >> 16 & 255) is b
    uint8 _padding;

    PackedColor(uint8 _r = 255, uint8 _g = 255, uint8 _b = 255) : r(_r), g(_b), b(_b) {}
  };

  /* Relative chunk position of the block vertex. */
  glm::vec3 position;

  /* Normalized (-1 -> 1) normal-vector of this vertex. */
  glm::vec3 normal;

  /* Texture coordinates of this vertex. See BlockTextureAtlas::GetTextureCoord(). */
  glm::vec2 texCoord;

  /**/
  glm::vec3 color;

  BlockVertex() = default;

  /* Requires an index buffer of { 0, 1, 2, 3, 0, 2 } for the color interpolation to work correctly.
  @param _positions: vertex position
  @param _colors: Bitmasks for the RGB colors of the four corners in the quad. */
  BlockVertex(glm::vec3 _position, glm::vec3 _normal, glm::vec2 _texCoord, glm::vec3 _color)
    : position(_position), normal(_normal), texCoord(_texCoord), color(_color)
  {}

  /* Shifts the vertex position by a given amount. */
  forceinline void Shift(glm::vec3 shiftAmount) {
    position += shiftAmount;
  }
};
static_assert(sizeof(BlockVertex) ==
  sizeof(glm::vec3) + sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3)
  , "Block Vertex data is not tightly packed");

/* Block quad vertex position ordering goes top right, top left, bottom left, bottom right. Bottom left is relative (0, 0) */
struct BlockQuad
{
  BlockVertex vertices[4];

  BlockQuad() = default;

  /* Quad of 4 vertices. Calculates the normal given the 4 positions. */
  BlockQuad(const glm::vec3 positions[4], const glm::vec2 texCoords[4], const glm::vec3 colors[4])
  {
    const glm::vec3 normal = NormalFromQuadPoints(positions);

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

  /* Shifts the 4 vertices by a given amount. */
  forceinline void Shift(glm::vec3 shiftAmount) {
    vertices[0].Shift(shiftAmount);
    vertices[1].Shift(shiftAmount);
    vertices[2].Shift(shiftAmount);
    vertices[3].Shift(shiftAmount);
  }

};
static_assert(sizeof(BlockQuad) == sizeof(BlockVertex) * 4, "Block quad byte size must be equal to 4x BlockVertex");
