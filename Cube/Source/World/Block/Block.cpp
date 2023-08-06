#include "Block.h"
#include "../World.h"
#include "../Chunk/Chunk.h"
#include "../Chunk/ChunkRenderComponent.h"
#include "../WorldTransform.h"
#include "BlockVTable.h"
#include "BlockFactory.h"

//void Block::Destroy()
//{
//  OnDestroy();
//  delete this;
//}

//void Block::AddBlockMeshToChunkMesh(ChunkMesh* chunkMesh, Chunk* chunk, WorldPosition position, glm::vec3 vertexOffset) const
//{
//  EMeshType meshType = GetMeshType();
//  switch (meshType) {
//  case EMeshType::cube:
//    CreateCubeMesh(chunkMesh, chunk, position, vertexOffset);
//  case EMeshType::custom:
//    //return BlockMesh();
//    break;
//  }
//}
//
//void Block::SetLight(BlockLight newLight)
//{
//  light = newLight;
//}
//
//void Block::SetFacing(BlockFacing newFacing)
//{
//  facing = newFacing.facing;
//}
//
//bool Block::IsBuried(const Chunk* owningChunk, const MappedAdjacentChunks& adjacentChunks, WorldPosition worldPosition, BlockPosition blockPos) const
//{
//  if (!isSolid) return false; // If the block itself is not solid transparent, its not buried
//
//  if (!blockPos.IsOnChunkEdge()) { // If not on edge, just check within chunk.
//    const BlockPosition blockAdjacentUp = BlockPosition(blockPos.X(), blockPos.Y() + 1, blockPos.Z());
//    const BlockPosition blockAdjacentDown = BlockPosition(blockPos.X(), blockPos.Y() - 1, blockPos.Z());
//    const BlockPosition blockAdjacentNorth = BlockPosition(blockPos.X(), blockPos.Y(), blockPos.Z() - 1);
//    const BlockPosition blockAdjacentEast = BlockPosition(blockPos.X() - 1, blockPos.Y(), blockPos.Z());
//    const BlockPosition blockAdjacentSouth = BlockPosition(blockPos.X(), blockPos.Y(), blockPos.Z() + 1);
//    const BlockPosition blockAdjacentWest = BlockPosition(blockPos.X() + 1, blockPos.Y(), blockPos.Z());
//
//    if (!owningChunk->GetBlock(blockAdjacentUp)->isSolid
//      || !owningChunk->GetBlock(blockAdjacentDown)->isSolid
//      || !owningChunk->GetBlock(blockAdjacentNorth)->isSolid
//      || !owningChunk->GetBlock(blockAdjacentEast)->isSolid
//      || !owningChunk->GetBlock(blockAdjacentSouth)->isSolid
//      || !owningChunk->GetBlock(blockAdjacentWest)->isSolid
//      ) return false;
//    return true;
//  }
//
//  const WorldPosition adjacentUp = WorldPosition(worldPosition.x, worldPosition.y + 1, worldPosition.z);
//  const WorldPosition adjacentDown = WorldPosition(worldPosition.x, worldPosition.y - 1, worldPosition.z);
//  const WorldPosition adjacentNorth = WorldPosition(worldPosition.x, worldPosition.y, worldPosition.z - 1);
//  const WorldPosition adjacentEast = WorldPosition(worldPosition.x - 1, worldPosition.y, worldPosition.z);
//  const WorldPosition adjacentSouth = WorldPosition(worldPosition.x, worldPosition.y, worldPosition.z + 1);
//  const WorldPosition adjacentWest = WorldPosition(worldPosition.x + 1, worldPosition.y, worldPosition.z);
//
//  const Chunk* upChunk = adjacentChunks.GetChunk(adjacentUp.ToChunkPosition());
//  if (upChunk == nullptr) return false;
//  const Chunk* downChunk = adjacentChunks.GetChunk(adjacentDown.ToChunkPosition());
//  if (downChunk == nullptr) return false;
//  const Chunk* northChunk = adjacentChunks.GetChunk(adjacentNorth.ToChunkPosition());
//  if (northChunk == nullptr) return false;
//  const Chunk* eastChunk = adjacentChunks.GetChunk(adjacentEast.ToChunkPosition());
//  if (eastChunk == nullptr) return false;
//  const Chunk* southChunk = adjacentChunks.GetChunk(adjacentSouth.ToChunkPosition());
//  if (southChunk == nullptr) return false;
//  const Chunk* westChunk = adjacentChunks.GetChunk(adjacentWest.ToChunkPosition());
//  if (westChunk == nullptr) return false;
//
//  const Block* upBlock = upChunk->GetBlock(adjacentUp.ToBlockPosition());
//  const Block* downBlock = downChunk->GetBlock(adjacentDown.ToBlockPosition());
//  const Block* northBlock = northChunk->GetBlock(adjacentNorth.ToBlockPosition());
//  const Block* eastBlock = eastChunk->GetBlock(adjacentEast.ToBlockPosition());
//  const Block* southBlock = southChunk->GetBlock(adjacentSouth.ToBlockPosition());
//  const Block* westBlock = westChunk->GetBlock(adjacentWest.ToBlockPosition());
//
//  if(!upBlock->isSolid // If any adjacent blocks are transparent, its not buried
//    || !downBlock->isSolid
//    || !northBlock->isSolid
//    || !eastBlock->isSolid
//    || !southBlock->isSolid
//    || !westBlock->isSolid) return false;
//
//  return true;
//}
//
//static void FillPackedPositionBuffer(PackedBlockOffsetPosition* buffer, const glm::vec3 positions[4], const glm::vec3 vertexOffset) {
//  buffer[0] = PackedBlockOffsetPosition::Pack(positions[0] + vertexOffset);
//  buffer[1] = PackedBlockOffsetPosition::Pack(positions[1] + vertexOffset);
//  buffer[2] = PackedBlockOffsetPosition::Pack(positions[2] + vertexOffset);
//  buffer[3] = PackedBlockOffsetPosition::Pack(positions[3] + vertexOffset);
//}
//
//void Block::AddBlockMeshToChunkMeshBitmaskTest(ChunkMesh* chunkMesh, const Chunk* chunk, const WorldPosition position, const glm::vec3 vertexOffset, const MappedAdjacentAndBuriedChunks& adjacentChunks) const
//{
//  const EBlockTexture allSideTexture = GetAllSidedTexture();
//  const glm::vec2 texCoords[4] = {
//    BlockTextureAtlas::GetTextureCoord(allSideTexture, {0, 0}),
//    BlockTextureAtlas::GetTextureCoord(allSideTexture, {1, 0}),
//    BlockTextureAtlas::GetTextureCoord(allSideTexture, {1, 1}),
//    BlockTextureAtlas::GetTextureCoord(allSideTexture, {0, 1})
//  };
//  const PackedColor c000{ 255, 255, 255 };
//  const PackedColor c100{ 255, 255, 255 };
//  const PackedColor c010{ 255, 255, 255 };
//  const PackedColor c110{ 255, 255, 255 };
//  const PackedColor c001{ 255, 255, 255 };
//  const PackedColor c101{ 255, 255, 255 };
//  const PackedColor c011{ 255, 255, 255 };
//  const PackedColor c111{ 255, 255, 255 };
//
//  const glm::vec3 bottomPos[4] = {
//   glm::vec3(0, 0, 0),
//   glm::vec3(0, 0, 1),
//   glm::vec3(1, 0, 1),
//   glm::vec3(1, 0, 0)
//  };
//  const PackedColor bottomCols[4] = {
//    c000, c001, c101, c100
//  };
//
//  const glm::vec3 northPos[4] = {
//    glm::vec3(0, 0, 0),
//    glm::vec3(1, 0, 0),
//    glm::vec3(1, 1, 0),
//    glm::vec3(0, 1, 0)
//  };
//  const PackedColor northCols[4] = {
//    c000, c100, c110, c010
//  };
//
//  const glm::vec3 eastPos[4] = {
//    glm::vec3(0, 0, 1),
//    glm::vec3(0, 0, 0),
//    glm::vec3(0, 1, 0),
//    glm::vec3(0, 1, 1)
//  };
//  const PackedColor eastCols[4] = {
//    c001, c000, c010, c011
//  };
//
//  const glm::vec3 southPos[4] = {
//    glm::vec3(1, 0, 1),
//    glm::vec3(0, 0, 1),
//    glm::vec3(0, 1, 1),
//    glm::vec3(1, 1, 1)
//  };
//  const PackedColor southCols[4] = {
//    c101, c001, c011, c111
//  };
//
//  const glm::vec3 westPos[4] = {
//    glm::vec3(1, 0, 0),
//    glm::vec3(1, 0, 1),
//    glm::vec3(1, 1, 1),
//    glm::vec3(1, 1, 0)
//  };
//  const PackedColor westCols[4] = {
//    c100, c101, c111, c110
//  };
//
//  const glm::vec3 topPos[4] = {
//   glm::vec3(0, 1, 0),
//   glm::vec3(1, 1, 0),
//   glm::vec3(1, 1, 1),
//   glm::vec3(0, 1, 1)
//  };
//  const PackedColor topCols[4] = {
//    c010, c110, c111, c011
//  };
//
//
//  PackedBlockOffsetPosition packedPositionBuffer[4];
//
//  //const BlockPosition blockPos = position.ToBlockPosition();
//  //if (!blockPos.IsOnChunkEdge()) {
//  //  if (CanDrawFaceWithinChunk(chunk, position, BlockFacing::Dir_Down)) {
//  //    FillPackedPositionBuffer(packedPositionBuffer, bottomPos, vertexOffset);
//  //    const BlockQuad bottom = BlockQuad(packedPositionBuffer, texCoords, bottomCols);
//  //    chunkMesh->AddQuad(bottom);
//  //  }
//  //  if (CanDrawFaceWithinChunk(chunk, position, BlockFacing::Dir_North)) {
//  //    FillPackedPositionBuffer(packedPositionBuffer, northPos, vertexOffset);
//  //    const BlockQuad north = BlockQuad(packedPositionBuffer, texCoords, northCols);
//  //    chunkMesh->AddQuad(north);
//  //  }
//  //  if (CanDrawFaceWithinChunk(chunk, position, BlockFacing::Dir_East)) {
//  //    FillPackedPositionBuffer(packedPositionBuffer, eastPos, vertexOffset);
//  //    const BlockQuad east = BlockQuad(packedPositionBuffer, texCoords, eastCols);
//  //    chunkMesh->AddQuad(east);
//  //  }
//  //  if (CanDrawFaceWithinChunk(chunk, position, BlockFacing::Dir_South)) {
//  //    FillPackedPositionBuffer(packedPositionBuffer, southPos, vertexOffset);
//  //    const BlockQuad south = BlockQuad(packedPositionBuffer, texCoords, southCols);
//  //    chunkMesh->AddQuad(south);
//  //  }
//  //  if (CanDrawFaceWithinChunk(chunk, position, BlockFacing::Dir_West)) {
//  //    FillPackedPositionBuffer(packedPositionBuffer, westPos, vertexOffset);
//  //    const BlockQuad west = BlockQuad(packedPositionBuffer, texCoords, westCols);
//  //    chunkMesh->AddQuad(west);
//  //  }
//  //  if (CanDrawFaceWithinChunk(chunk, position, BlockFacing::Dir_Up)) {
//  //    FillPackedPositionBuffer(packedPositionBuffer, topPos, vertexOffset);
//  //    const BlockQuad top = BlockQuad(packedPositionBuffer, texCoords, topCols);
//  //    chunkMesh->AddQuad(top);
//  //  }
//  //  return;
//  //}
//
//  if (CanDrawFace(adjacentChunks, position, BlockFacing::Dir_Down)) {
//    FillPackedPositionBuffer(packedPositionBuffer, bottomPos, vertexOffset);
//    const BlockQuad bottom = BlockQuad(packedPositionBuffer, texCoords, bottomCols);
//    chunkMesh->AddQuad(bottom);
//  }
//  if (CanDrawFace(adjacentChunks, position, BlockFacing::Dir_North)) {
//    FillPackedPositionBuffer(packedPositionBuffer, northPos, vertexOffset);
//    const BlockQuad north = BlockQuad(packedPositionBuffer, texCoords, northCols);
//    chunkMesh->AddQuad(north);
//  }
//  if (CanDrawFace(adjacentChunks, position, BlockFacing::Dir_East)) {
//    FillPackedPositionBuffer(packedPositionBuffer, eastPos, vertexOffset);
//    const BlockQuad east = BlockQuad(packedPositionBuffer, texCoords, eastCols);
//    chunkMesh->AddQuad(east);
//  }
//  if (CanDrawFace(adjacentChunks, position, BlockFacing::Dir_South)) {
//    FillPackedPositionBuffer(packedPositionBuffer, southPos, vertexOffset);
//    const BlockQuad south = BlockQuad(packedPositionBuffer, texCoords, southCols);
//    chunkMesh->AddQuad(south);
//  }
//  if (CanDrawFace(adjacentChunks, position, BlockFacing::Dir_West)) {
//    FillPackedPositionBuffer(packedPositionBuffer, westPos, vertexOffset);
//    const BlockQuad west = BlockQuad(packedPositionBuffer, texCoords, westCols);
//    chunkMesh->AddQuad(west);
//  }
//  if (CanDrawFace(adjacentChunks, position, BlockFacing::Dir_Up)) {
//    FillPackedPositionBuffer(packedPositionBuffer, topPos, vertexOffset);
//    const BlockQuad top = BlockQuad(packedPositionBuffer, texCoords, topCols);
//    chunkMesh->AddQuad(top);
//  }
//}
//
//void Block::OnDestroy()
//{
//}
//
//void Block::CreateCubeMesh(ChunkMesh* chunkMesh, Chunk* chunk, WorldPosition position, glm::vec3 vertexOffset) const
//{
//  const EBlockTexture allSideTexture = GetAllSidedTexture();
//  const glm::vec2 texCoords[4] = {
//    BlockTextureAtlas::GetTextureCoord(allSideTexture, {0, 0}),
//    BlockTextureAtlas::GetTextureCoord(allSideTexture, {1, 0}),
//    BlockTextureAtlas::GetTextureCoord(allSideTexture, {1, 1}),
//    BlockTextureAtlas::GetTextureCoord(allSideTexture, {0, 1})
//  };
//
//  const PackedColor c000{ 255, 255, 255 };
//  const PackedColor c100{ 255, 255, 255 };
//  const PackedColor c010{ 255, 255, 255 };
//  const PackedColor c110{ 255, 255, 255 };
//  const PackedColor c001{ 255, 255, 255 };
//  const PackedColor c101{ 255, 255, 255 };
//  const PackedColor c011{ 255, 255, 255 };
//  const PackedColor c111{ 255, 255, 255 };
//
//  const glm::vec3 bottomPos[4] = {
//   glm::vec3(0, 0, 0),
//   glm::vec3(0, 0, 1),
//   glm::vec3(1, 0, 1),
//   glm::vec3(1, 0, 0)
//  };
//  const PackedColor bottomCols[4] = {
//    c000, c001, c101, c100
//  };
//
//  const glm::vec3 northPos[4] = {
//    glm::vec3(0, 0, 0),
//    glm::vec3(1, 0, 0),
//    glm::vec3(1, 1, 0),
//    glm::vec3(0, 1, 0)
//  };
//  const PackedColor northCols[4] = {
//    c000, c100, c110, c010
//  };
//
//  const glm::vec3 eastPos[4] = {
//    glm::vec3(0, 0, 1),
//    glm::vec3(0, 0, 0),
//    glm::vec3(0, 1, 0),
//    glm::vec3(0, 1, 1)
//  };
//  const PackedColor eastCols[4] = {
//    c001, c000, c010, c011
//  };
//
//  const glm::vec3 southPos[4] = {
//    glm::vec3(1, 0, 1),
//    glm::vec3(0, 0, 1),
//    glm::vec3(0, 1, 1),
//    glm::vec3(1, 1, 1)
//  };
//  const PackedColor southCols[4] = {
//    c101, c001, c011, c111
//  };
//
//  const glm::vec3 westPos[4] = {
//    glm::vec3(1, 0, 0),
//    glm::vec3(1, 0, 1),
//    glm::vec3(1, 1, 1),
//    glm::vec3(1, 1, 0)
//  };
//  const PackedColor westCols[4] = {
//    c100, c101, c111, c110
//  };
//
//  const glm::vec3 topPos[4] = {
//   glm::vec3(0, 1, 0),
//   glm::vec3(1, 1, 0),
//   glm::vec3(1, 1, 1),
//   glm::vec3(0, 1, 1)
//  };
//  const PackedColor topCols[4] = {
//    c010, c110, c111, c011
//  };
//
//  PackedBlockOffsetPosition packedPositionBuffer[4];
//  
//  if (CanDrawFace(position, BlockFacing::Dir_Down)) {
//    FillPackedPositionBuffer(packedPositionBuffer, bottomPos, vertexOffset);
//    const BlockQuad bottom = BlockQuad(packedPositionBuffer, texCoords, bottomCols);
//    chunkMesh->AddQuad(bottom);
//  }
//  if (CanDrawFace(position, BlockFacing::Dir_North)) {
//    FillPackedPositionBuffer(packedPositionBuffer, northPos, vertexOffset);
//    const BlockQuad north = BlockQuad(packedPositionBuffer, texCoords, northCols);
//    chunkMesh->AddQuad(north);
//  }
//  if (CanDrawFace(position, BlockFacing::Dir_East)) {
//    FillPackedPositionBuffer(packedPositionBuffer, eastPos, vertexOffset);
//    const BlockQuad east = BlockQuad(packedPositionBuffer, texCoords, eastCols);
//    chunkMesh->AddQuad(east);
//  }
//  if (CanDrawFace(position, BlockFacing::Dir_South)) {
//    FillPackedPositionBuffer(packedPositionBuffer, southPos, vertexOffset);
//    const BlockQuad south = BlockQuad(packedPositionBuffer, texCoords, southCols);
//    chunkMesh->AddQuad(south);
//  }
//  if (CanDrawFace(position, BlockFacing::Dir_West)) {
//    FillPackedPositionBuffer(packedPositionBuffer, westPos, vertexOffset);
//    const BlockQuad west = BlockQuad(packedPositionBuffer, texCoords, westCols);
//    chunkMesh->AddQuad(west);
//  }
//  if (CanDrawFace(position, BlockFacing::Dir_Up)) {
//    FillPackedPositionBuffer(packedPositionBuffer, topPos, vertexOffset);
//    const BlockQuad top = BlockQuad(packedPositionBuffer, texCoords, topCols);
//    chunkMesh->AddQuad(top);
//  }
//  
//}
//
//#define DRAW_FACE_IF_NULL_ADJACENT true
//
//bool Block::CanDrawFace(WorldPosition position, BlockFacing face) const
//{
//  const WorldPosition adjacentPosition = position.Adjacent(face);
//
//  const Block* adjacentBlock = GetWorld()->GetBlock(adjacentPosition);
//  if (adjacentBlock == nullptr) {
//    return DRAW_FACE_IF_NULL_ADJACENT;
//  }
//  
//  const Block::ETransparency adjacentTransparency = adjacentBlock->GetFaceTransparency(face.Opposite());
//  switch (adjacentTransparency) {
//  case Block::ETransparency::solid:
//    return false;
//  case Block::ETransparency::transparent:
//    return true;
//  case Block::ETransparency::custom:
//    // TODO: Implement something for custom mesh transparency idk
//    return true;
//  default:
//    gk_assertm(false, "Unreachable code block. Mesh transparency type must be one of the enum class values within Block::ETransparency");
//    return true;
//  }
//}
//
//bool Block::CanDrawFace(const MappedAdjacentAndBuriedChunks& adjacentChunks, WorldPosition position, BlockFacing face) const
//{
//  const WorldPosition adjacentPosition = position.Adjacent(face);
//  const Chunk* chunkWithBlock = adjacentChunks.GetChunk(adjacentPosition.ToChunkPosition());
//  if (chunkWithBlock == nullptr) return DRAW_FACE_IF_NULL_ADJACENT;
//
//  const Block* adjacentBlock = chunkWithBlock->GetBlock(adjacentPosition.ToBlockPosition());
//  //return adjacentBlock->solidSides.IsFacing(face.Opposite().facing);
//  //const Block* adjacentBlock = GetWorld()->GetBlock(adjacentPosition);
//  //if (adjacentBlock == nullptr) {
//  //  return DRAW_FACE_IF_NULL_ADJACENT;
//  //}
//  const ETransparency adjacentTransparency = adjacentBlock->GetFaceTransparency(face.Opposite());
//  switch (adjacentTransparency) {
//  case Block::ETransparency::solid:
//    return false;
//  case Block::ETransparency::transparent:
//    return true;
//  case Block::ETransparency::custom:
//    // TODO: Implement something for custom mesh transparency idk
//    return true;
//  default:
//    gk_assertm(false, "Unreachable code block. Mesh transparency type must be one of the enum class values within Block::ETransparency");
//    return true;
//  }
//}
//
//bool Block::CanDrawFaceWithinChunk(const Chunk* chunk, const WorldPosition position, const BlockFacing face) const
//{
//  const BlockPosition adjacentBlockPosition = position.Adjacent(face).ToBlockPosition();
//  const Block* adjacentBlock = chunk->GetBlock(adjacentBlockPosition); 
//  //return adjacentBlock->solidSides.IsFacing(face.Opposite().facing);
//
//  const ETransparency adjacentTransparency = adjacentBlock->GetFaceTransparency(face.Opposite());
//  switch (adjacentTransparency) {
//  case Block::ETransparency::solid:
//    return false;
//  case Block::ETransparency::transparent:
//    return true;
//  case Block::ETransparency::custom:
//    // TODO: Implement something for custom mesh transparency idk
//    return true;
//  default:
//    gk_assertm(false, "Unreachable code block. Mesh transparency type must be one of the enum class values within Block::ETransparency");
//    return false;
//  }
//}

Block::Block()
{
  vTable = nullptr;
  _emptyFutureBlockDataComponent = nullptr;
  facing = 0;
  isPendingDelete = false;
  isFullSolid = true;
}

Block::~Block()
{
  // DO NOT destroy the block data component here.
}

Block::Block(const Block& other)
{
  vTable = other.vTable;
  gk_assertNotNull(other.vTable);
  _emptyFutureBlockDataComponent = other._emptyFutureBlockDataComponent;
  facing = other.facing;
  isPendingDelete = other.isPendingDelete;
  isFullSolid = other.isFullSolid;
}

Block& Block::operator=(const Block& other)
{
  vTable = other.vTable;
  gk_assertNotNull(other.vTable);
  _emptyFutureBlockDataComponent = other._emptyFutureBlockDataComponent;
  facing = other.facing;
  isPendingDelete = other.isPendingDelete;
  isFullSolid = other.isFullSolid;
  return *this;
}

void Block::Destroy()
{
  isPendingDelete = true;
}

GlobalString Block::GetName() const
{
  return vTable->blockName;
}

bool Block::IsBuried(const Chunk* owningChunk, const MappedAdjacentChunks& adjacentChunks, WorldPosition worldPosition, BlockPosition blockPos) const
{
  if (!isFullSolid) return false; // If the block itself is not solid transparent, its not buried

  if (!blockPos.IsOnChunkEdge()) { // If not on edge, just check within chunk.
    const BlockPosition blockAdjacentUp = BlockPosition(blockPos.X(), blockPos.Y() + 1, blockPos.Z());
    const BlockPosition blockAdjacentDown = BlockPosition(blockPos.X(), blockPos.Y() - 1, blockPos.Z());
    const BlockPosition blockAdjacentNorth = BlockPosition(blockPos.X(), blockPos.Y(), blockPos.Z() - 1);
    const BlockPosition blockAdjacentEast = BlockPosition(blockPos.X() - 1, blockPos.Y(), blockPos.Z());
    const BlockPosition blockAdjacentSouth = BlockPosition(blockPos.X(), blockPos.Y(), blockPos.Z() + 1);
    const BlockPosition blockAdjacentWest = BlockPosition(blockPos.X() + 1, blockPos.Y(), blockPos.Z());

    if (!owningChunk->GetBlock(blockAdjacentUp)->isFullSolid
      || !owningChunk->GetBlock(blockAdjacentDown)->isFullSolid
      || !owningChunk->GetBlock(blockAdjacentNorth)->isFullSolid
      || !owningChunk->GetBlock(blockAdjacentEast)->isFullSolid
      || !owningChunk->GetBlock(blockAdjacentSouth)->isFullSolid
      || !owningChunk->GetBlock(blockAdjacentWest)->isFullSolid
      ) return false;
    return true;
  }

  const WorldPosition adjacentUp = WorldPosition(worldPosition.x, worldPosition.y + 1, worldPosition.z);
  const WorldPosition adjacentDown = WorldPosition(worldPosition.x, worldPosition.y - 1, worldPosition.z);
  const WorldPosition adjacentNorth = WorldPosition(worldPosition.x, worldPosition.y, worldPosition.z - 1);
  const WorldPosition adjacentEast = WorldPosition(worldPosition.x - 1, worldPosition.y, worldPosition.z);
  const WorldPosition adjacentSouth = WorldPosition(worldPosition.x, worldPosition.y, worldPosition.z + 1);
  const WorldPosition adjacentWest = WorldPosition(worldPosition.x + 1, worldPosition.y, worldPosition.z);

  const Chunk* upChunk = adjacentChunks.GetChunk(adjacentUp.ToChunkPosition());
  if (upChunk == nullptr) return false;
  const Chunk* downChunk = adjacentChunks.GetChunk(adjacentDown.ToChunkPosition());
  if (downChunk == nullptr) return false;
  const Chunk* northChunk = adjacentChunks.GetChunk(adjacentNorth.ToChunkPosition());
  if (northChunk == nullptr) return false;
  const Chunk* eastChunk = adjacentChunks.GetChunk(adjacentEast.ToChunkPosition());
  if (eastChunk == nullptr) return false;
  const Chunk* southChunk = adjacentChunks.GetChunk(adjacentSouth.ToChunkPosition());
  if (southChunk == nullptr) return false;
  const Chunk* westChunk = adjacentChunks.GetChunk(adjacentWest.ToChunkPosition());
  if (westChunk == nullptr) return false;

  const Block* upBlock = upChunk->GetBlock(adjacentUp.ToBlockPosition());
  const Block* downBlock = downChunk->GetBlock(adjacentDown.ToBlockPosition());
  const Block* northBlock = northChunk->GetBlock(adjacentNorth.ToBlockPosition());
  const Block* eastBlock = eastChunk->GetBlock(adjacentEast.ToBlockPosition());
  const Block* southBlock = southChunk->GetBlock(adjacentSouth.ToBlockPosition());
  const Block* westBlock = westChunk->GetBlock(adjacentWest.ToBlockPosition());

  if(!upBlock->isFullSolid // If any adjacent blocks are transparent, its not buried
    || !downBlock->isFullSolid
    || !northBlock->isFullSolid
    || !eastBlock->isFullSolid
    || !southBlock->isFullSolid
    || !westBlock->isFullSolid) return false;

  return true;
}

void Block::ConstructMesh(ChunkMesh* mesh, const Chunk* chunk, const WorldPosition position, const glm::vec3 vertexOffset, const MappedAdjacentAndBuriedChunks& adjacentChunks) const
{
  vTable->meshFunc(this, mesh, chunk, position, vertexOffset, adjacentChunks);
}
