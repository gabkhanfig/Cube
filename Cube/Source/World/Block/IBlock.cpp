#include "IBlock.h"
#include "../World.h"

void IBlock::AddBlockMeshToChunkMesh(ChunkMesh* chunkMesh, Chunk* chunk, WorldPosition position, glm::vec3 vertexOffset) const
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

void IBlock::CreateCubeMesh(ChunkMesh* chunkMesh, Chunk* chunk, WorldPosition position, glm::vec3 vertexOffset) const
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

  if (CanDrawFace(position, BlockFacing::Dir_Down)) {
    BlockQuad bottom = BlockQuad(bottomPos, texCoords, bottomCols);
    bottom.Shift(vertexOffset);
    chunkMesh->AddQuad(bottom);
  }
  if (CanDrawFace(position, BlockFacing::Dir_North)) {
    BlockQuad north = BlockQuad(northPos, texCoords, northCols);
    north.Shift(vertexOffset);
    chunkMesh->AddQuad(north);
  }
  if (CanDrawFace(position, BlockFacing::Dir_East)) {
    BlockQuad east = BlockQuad(eastPos, texCoords, eastCols);
    east.Shift(vertexOffset);
    chunkMesh->AddQuad(east);
  }
  if (CanDrawFace(position, BlockFacing::Dir_South)) {
    BlockQuad south = BlockQuad(southPos, texCoords, southCols);
    south.Shift(vertexOffset);
    chunkMesh->AddQuad(south);
  }
  if (CanDrawFace(position, BlockFacing::Dir_West)) {
    BlockQuad west = BlockQuad(westPos, texCoords, westCols);
    west.Shift(vertexOffset);
    chunkMesh->AddQuad(west);
  }
  if (CanDrawFace(position, BlockFacing::Dir_Up)) {
    BlockQuad top = BlockQuad(topPos, texCoords, topCols);
    top.Shift(vertexOffset);
    chunkMesh->AddQuad(top);
  }
  
}

bool IBlock::CanDrawFace(WorldPosition position, BlockFacing face) const
{
  const WorldPosition adjacentPosition = position.Adjacent(face);

#define DRAW_FACE_IF_NULL_ADJACENT true

  const IBlock* adjacentBlock = GetWorld()->GetBlock(adjacentPosition);
  if (adjacentBlock == nullptr) {
    return DRAW_FACE_IF_NULL_ADJACENT;
  }
  
  const IBlock::EMeshTransparency adjacentTransparency = adjacentBlock->GetFaceTransparency(face.Opposite());
  switch (adjacentTransparency) {
  case IBlock::EMeshTransparency::solid:
    return false;
  case IBlock::EMeshTransparency::transparent:
    return true;
  case IBlock::EMeshTransparency::custom:
    // TODO: Implement something for custom mesh transparency idk
    return true;
  default:
    checkm(false, "Unreachable code block. Mesh transparency type must be one of the enum class values within IBlcok::EMeshTransparency");
    return true;
  }
}

