#include "IBlock.h"

void IBlock::AddBlockMeshToChunkMesh(ChunkMesh& chunkMesh, Chunk* chunk, WorldPosition position) const
{
  EMeshType meshType = GetMeshType();
  switch (meshType) {
  case EMeshType::cube:
    CreateCubeMesh(chunkMesh, chunk, position);
  case EMeshType::custom:
    //return BlockMesh();
    break;
  }
}

void IBlock::OnDestroy()
{
}

void IBlock::CreateCubeMesh(ChunkMesh& chunkMesh, Chunk* chunk, WorldPosition position) const
{
  const EBlockTexture allSideTexture = GetAllSidedTexture();
  const glm::vec2 texCoords[4] = {
    BlockTextureAtlas::GetTextureCoord(allSideTexture, {0, 0}),
    BlockTextureAtlas::GetTextureCoord(allSideTexture, {1, 0}),
    BlockTextureAtlas::GetTextureCoord(allSideTexture, {1, 1}),
    BlockTextureAtlas::GetTextureCoord(allSideTexture, {0, 1})
  };

  const glm::vec3 c000{ 0, 0, 0 };
  const glm::vec3 c100{ 1, 0, 0 };
  const glm::vec3 c010{ 0, 1, 0 };
  const glm::vec3 c110{ 1, 1, 0 };
  const glm::vec3 c001{ 0, 0, 1 };
  const glm::vec3 c101{ 1, 0, 1 };
  const glm::vec3 c011{ 0, 1, 1 };
  const glm::vec3 c111{ 1, 1, 1 };

  const glm::vec3 bottomPos[4] = {
   glm::vec3(0, 0, 0),
   glm::vec3(0, 0, 1),
   glm::vec3(1, 0, 1),
   glm::vec3(1, 0, 0)
  };
  const glm::vec3 bottomCols[4] = {
    c000, c001, c101, c100
  };

  const glm::vec3 northPos[4] = {
    glm::vec3(0, 0, 0),
    glm::vec3(1, 0, 0),
    glm::vec3(1, 1, 0),
    glm::vec3(0, 1, 0)
  };
  const glm::vec3 northCols[4] = {
    c000, c100, c110, c010
  };

  const glm::vec3 eastPos[4] = {
    glm::vec3(0, 0, 1),
    glm::vec3(0, 0, 0),
    glm::vec3(0, 1, 0),
    glm::vec3(0, 1, 1)
  };
  const glm::vec3 eastCols[4] = {
    c001, c000, c010, c011
  };

  const glm::vec3 southPos[4] = {
    glm::vec3(1, 0, 1),
    glm::vec3(0, 0, 1),
    glm::vec3(0, 1, 1),
    glm::vec3(1, 1, 1)
  };
  const glm::vec3 southCols[4] = {
    c101, c001, c011, c111
  };

  const glm::vec3 westPos[4] = {
    glm::vec3(1, 0, 0),
    glm::vec3(1, 0, 1),
    glm::vec3(1, 1, 1),
    glm::vec3(1, 1, 0)
  };
  const glm::vec3 westCols[4] = {
    c100, c101, c111, c110
  };

  const glm::vec3 topPos[4] = {
   glm::vec3(0, 1, 0),
   glm::vec3(1, 1, 0),
   glm::vec3(1, 1, 1),
   glm::vec3(0, 1, 1)
  };
  const glm::vec3 topCols[4] = {
    c010, c110, c111, c011
  };

  const BlockPosition blockPos = position.ToBlockPosition();
  const glm::vec3 shift = glm::vec3(blockPos.x, blockPos.y, blockPos.z);

  BlockQuad bottom = BlockQuad(bottomPos, texCoords, bottomCols);
  bottom.Shift(shift);
  BlockQuad north = BlockQuad(northPos, texCoords, northCols);
  north.Shift(shift);
  BlockQuad east = BlockQuad(eastPos, texCoords, eastCols);
  east.Shift(shift);
  BlockQuad south = BlockQuad(southPos, texCoords, southCols);
  south.Shift(shift);
  BlockQuad west = BlockQuad(westPos, texCoords, westCols);
  west.Shift(shift);
  BlockQuad top = BlockQuad(topPos, texCoords, topCols);
  top.Shift(shift);

  chunkMesh.AddQuad(bottom);
  chunkMesh.AddQuad(north);
  chunkMesh.AddQuad(east);
  chunkMesh.AddQuad(south);
  chunkMesh.AddQuad(west);
  chunkMesh.AddQuad(top);
}

