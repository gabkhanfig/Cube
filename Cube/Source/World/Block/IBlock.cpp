#include "IBlock.h"
#include "../World.h"

void IBlock::AddBlockMeshToChunkMesh(ChunkMesh& chunkMesh, Chunk* chunk, WorldPosition position, glm::vec3 vertexOffset) const
{
  EMeshType meshType = GetMeshType();
  switch (meshType) {
  case EMeshType::cube:
    CreateCubeMesh(chunkMesh, chunk, position, vertexOffset);
  case EMeshType::custom:
    //return BlockMesh();
    break;
  }
}

void IBlock::OnDestroy()
{
}

void IBlock::CreateCubeMesh(ChunkMesh& chunkMesh, Chunk* chunk, WorldPosition position, glm::vec3 vertexOffset) const
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

#define DRAW_FACE_IF_NULL_ADJACENT true

  IBlock* adjacentBottom = GetWorld()->GetBlock(WorldPosition(position.x, position.y - 1, position.z));
  //IBlock* adjacentBottom = GetBlockNextTo(position, BlockFacing::Direction::Dir_Down);
  IBlock* adjacentNorth = GetWorld()->GetBlock(WorldPosition(position.x, position.y, position.z - 1));
  IBlock* adjacentEast = GetWorld()->GetBlock(WorldPosition(position.x - 1, position.y, position.z));
  IBlock* adjacentSouth = GetWorld()->GetBlock(WorldPosition(position.x, position.y, position.z + 1));
  IBlock* adjacentWest = GetWorld()->GetBlock(WorldPosition(position.x + 1, position.y, position.z));
  IBlock* adjacentTop = GetWorld()->GetBlock(WorldPosition(position.x, position.y + 1, position.z));

#if DRAW_FACE_IF_NULL_ADJACENT == true
  const bool canDrawBottom = adjacentBottom == nullptr;
  const bool canDrawNorth = adjacentNorth == nullptr;
  const bool canDrawEast = adjacentEast == nullptr;
  const bool canDrawSouth = adjacentSouth == nullptr;
  const bool canDrawWest = adjacentWest == nullptr;
  const bool canDrawTop = adjacentTop == nullptr;
#else

#endif

  if (canDrawBottom) {
    BlockQuad bottom = BlockQuad(bottomPos, texCoords, bottomCols);
    bottom.Shift(vertexOffset);
    chunkMesh.AddQuad(bottom);
  }
  if (canDrawNorth) {
    BlockQuad north = BlockQuad(northPos, texCoords, northCols);
    north.Shift(vertexOffset);
    chunkMesh.AddQuad(north);
  }
  if (canDrawEast) {
    BlockQuad east = BlockQuad(eastPos, texCoords, eastCols);
    east.Shift(vertexOffset);
    chunkMesh.AddQuad(east);
  }
  if (canDrawSouth) {
    BlockQuad south = BlockQuad(southPos, texCoords, southCols);
    south.Shift(vertexOffset);
    chunkMesh.AddQuad(south);
  }
  if (canDrawWest) {
    BlockQuad west = BlockQuad(westPos, texCoords, westCols);
    west.Shift(vertexOffset);
    chunkMesh.AddQuad(west);
  }
  if (canDrawTop) {
    BlockQuad top = BlockQuad(topPos, texCoords, topCols);
    top.Shift(vertexOffset);
    chunkMesh.AddQuad(top);
  }
  
}

//IBlock* IBlock::GetBlockNextTo(WorldPosition thisPosition, BlockFacing facing) const
//{
//  WorldPosition otherPosition = (
//    thisPosition.x,// - bool(facing.facing == BlockFacing::Direction::Dir_East) + bool(facing.facing == BlockFacing::Direction::Dir_West),
//    thisPosition.y - 10,//bool(facing.facing == BlockFacing::Direction::Dir_Down) + bool(facing.facing == BlockFacing::Direction::Dir_Up),
//    thisPosition.z// - bool(facing.facing == BlockFacing::Direction::Dir_North) + bool(facing.facing == BlockFacing::Direction::Dir_South)
//    );
//  return GetWorld()->GetBlock(otherPosition);
//}

