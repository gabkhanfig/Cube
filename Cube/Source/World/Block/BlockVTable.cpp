#include "BlockVTable.h"
#include "Block.h"
#include "../World.h"
#include "../Chunk/Chunk.h"
#include "../Chunk/ChunkRenderComponent.h"
#include "../WorldTransform.h"

BlockVTable::BlockVTable(GlobalString name)
{
  blockName = name;
  meshFunc = nullptr;
  getFaceTexture = nullptr;
}

BlockVTable* BlockVTable::Default(GlobalString name)
{
  BlockVTable* vtable = new BlockVTable(name);
  vtable->meshFunc = FullCubeMeshFunc;
  return vtable;
}

static void FillPackedPositionBuffer(PackedBlockOffsetPosition* buffer, const glm::vec3 positions[4], const glm::vec3 vertexOffset) {
  buffer[0] = PackedBlockOffsetPosition::Pack(positions[0] + vertexOffset);
  buffer[1] = PackedBlockOffsetPosition::Pack(positions[1] + vertexOffset);
  buffer[2] = PackedBlockOffsetPosition::Pack(positions[2] + vertexOffset);
  buffer[3] = PackedBlockOffsetPosition::Pack(positions[3] + vertexOffset);
}

static void FillPositionBuffer(glm::vec3* buffer, const glm::vec3 positions[4], const glm::vec3 vertexOffset) {
  buffer[0] = positions[0] + vertexOffset;
  buffer[1] = positions[1] + vertexOffset;
  buffer[2] = positions[2] + vertexOffset;
  buffer[3] = positions[3] + vertexOffset;
}


static bool CanDrawFace(const MappedAdjacentAndBuriedChunks& adjacentChunks, WorldPosition position, BlockFacing face) {
  const WorldPosition adjacentPosition = position.Adjacent(face);
  const Chunk* chunkWithBlock = adjacentChunks.GetChunk(adjacentPosition.ToChunkPosition());
  if (chunkWithBlock == nullptr) return true; // draw face is null adjacent

  const Block* adjacentBlock = chunkWithBlock->GetBlock(adjacentPosition.ToBlockPosition());
  return !adjacentBlock->isFullSolid;
  //return adjacentBlock->solidSides.IsFacing(face.Opposite().facing);
  //const Block* adjacentBlock = GetWorld()->GetBlock(adjacentPosition);
  //if (adjacentBlock == nullptr) {
  //  return DRAW_FACE_IF_NULL_ADJACENT;
  //}
  //const ETransparency adjacentTransparency = adjacentBlock->GetFaceTransparency(face.Opposite());
  //switch (adjacentTransparency) {
  //case Block::ETransparency::solid:
  //  return false;
  //case Block::ETransparency::transparent:
  //  return true;
  //case Block::ETransparency::custom:
  //  // TODO: Implement something for custom mesh transparency idk
  //  return true;
  //default:
  //  gk_assertm(false, "Unreachable code block. Mesh transparency type must be one of the enum class values within Block::ETransparency");
  //  return true;
  //}
}

void FullCubeMeshFunc(const Block* self, ChunkMesh* mesh, const Chunk* chunk, const WorldPosition position, const glm::vec3 vertexOffset, const MappedAdjacentAndBuriedChunks& adjacentChunks)
{
  const EBlockTexture allSideTexture = self->vTable->getFaceTexture(self, BlockFacing::Dir_Down);
  const glm::vec2 texCoords[4] = {
    BlockTextureAtlas::GetTextureCoord(allSideTexture, {0, 0}),
    BlockTextureAtlas::GetTextureCoord(allSideTexture, {1, 0}),
    BlockTextureAtlas::GetTextureCoord(allSideTexture, {1, 1}),
    BlockTextureAtlas::GetTextureCoord(allSideTexture, {0, 1})
  };
  const PackedColor c000{ 255, 255, 255 };
  const PackedColor c100{ 255, 255, 255 };
  const PackedColor c010{ 255, 255, 255 };
  const PackedColor c110{ 255, 255, 255 };
  const PackedColor c001{ 255, 255, 255 };
  const PackedColor c101{ 255, 255, 255 };
  const PackedColor c011{ 255, 255, 255 };
  const PackedColor c111{ 255, 255, 255 };

  const glm::vec3 bottomPos[4] = {
   glm::vec3(1, 0, 1),
   glm::vec3(1, 0, 0),
   glm::vec3(0, 0, 0),
   glm::vec3(0, 0, 1)
  };
  const PackedColor bottomCols[4] = {
    c000, c001, c101, c100
  };

  const glm::vec3 northPos[4] = {
    glm::vec3(1, 1, 0),
    glm::vec3(0, 1, 0),
    glm::vec3(0, 0, 0),
    glm::vec3(1, 0, 0)
  };
  const PackedColor northCols[4] = {
    c000, c100, c110, c010
  };

  const glm::vec3 eastPos[4] = {
    glm::vec3(0, 0, 1),
    glm::vec3(0, 0, 0),
    glm::vec3(0, 1, 0),
    glm::vec3(0, 1, 1)
  };
  const PackedColor eastCols[4] = {
    c001, c000, c010, c011
  };

  const glm::vec3 southPos[4] = {
    glm::vec3(1, 0, 1),
    glm::vec3(0, 0, 1),
    glm::vec3(0, 1, 1),
    glm::vec3(1, 1, 1)
  };
  const PackedColor southCols[4] = {
    c101, c001, c011, c111
  };

  const glm::vec3 westPos[4] = {
    glm::vec3(1, 0, 0),
    glm::vec3(1, 0, 1),
    glm::vec3(1, 1, 1),
    glm::vec3(1, 1, 0)
  };
  const PackedColor westCols[4] = {
    c100, c101, c111, c110
  };

  const glm::vec3 topPos[4] = {
   glm::vec3(0, 1, 0),
   glm::vec3(1, 1, 0),
   glm::vec3(1, 1, 1),
   glm::vec3(0, 1, 1)
  };
  const PackedColor topCols[4] = {
    c010, c110, c111, c011
  };


  PackedBlockOffsetPosition packedPositionBuffer[4];
  glm::vec3 positionBuffer[4];

  //const BlockPosition blockPos = position.ToBlockPosition();
  //if (!blockPos.IsOnChunkEdge()) {
  //  if (CanDrawFaceWithinChunk(chunk, position, BlockFacing::Dir_Down)) {
  //    FillPackedPositionBuffer(packedPositionBuffer, bottomPos, vertexOffset);
  //    const BlockQuad bottom = BlockQuad(packedPositionBuffer, texCoords, bottomCols);
  //    chunkMesh->AddQuad(bottom);
  //  }
  //  if (CanDrawFaceWithinChunk(chunk, position, BlockFacing::Dir_North)) {
  //    FillPackedPositionBuffer(packedPositionBuffer, northPos, vertexOffset);
  //    const BlockQuad north = BlockQuad(packedPositionBuffer, texCoords, northCols);
  //    chunkMesh->AddQuad(north);
  //  }
  //  if (CanDrawFaceWithinChunk(chunk, position, BlockFacing::Dir_East)) {
  //    FillPackedPositionBuffer(packedPositionBuffer, eastPos, vertexOffset);
  //    const BlockQuad east = BlockQuad(packedPositionBuffer, texCoords, eastCols);
  //    chunkMesh->AddQuad(east);
  //  }
  //  if (CanDrawFaceWithinChunk(chunk, position, BlockFacing::Dir_South)) {
  //    FillPackedPositionBuffer(packedPositionBuffer, southPos, vertexOffset);
  //    const BlockQuad south = BlockQuad(packedPositionBuffer, texCoords, southCols);
  //    chunkMesh->AddQuad(south);
  //  }
  //  if (CanDrawFaceWithinChunk(chunk, position, BlockFacing::Dir_West)) {
  //    FillPackedPositionBuffer(packedPositionBuffer, westPos, vertexOffset);
  //    const BlockQuad west = BlockQuad(packedPositionBuffer, texCoords, westCols);
  //    chunkMesh->AddQuad(west);
  //  }
  //  if (CanDrawFaceWithinChunk(chunk, position, BlockFacing::Dir_Up)) {
  //    FillPackedPositionBuffer(packedPositionBuffer, topPos, vertexOffset);
  //    const BlockQuad top = BlockQuad(packedPositionBuffer, texCoords, topCols);
  //    chunkMesh->AddQuad(top);
  //  }
  //  return;
  //}

  if (CanDrawFace(adjacentChunks, position, BlockFacing::Dir_Down)) {
    //FillPackedPositionBuffer(packedPositionBuffer, bottomPos, vertexOffset);
    FillPositionBuffer(positionBuffer, bottomPos, vertexOffset);
    const BlockQuad bottom = BlockQuad(positionBuffer, texCoords, bottomCols);
    mesh->AddQuad(bottom);
  }
  if (CanDrawFace(adjacentChunks, position, BlockFacing::Dir_North)) {
    //FillPackedPositionBuffer(packedPositionBuffer, northPos, vertexOffset);
    FillPositionBuffer(positionBuffer, northPos, vertexOffset);
    const BlockQuad north = BlockQuad(positionBuffer, texCoords, northCols);
    mesh->AddQuad(north);
  }
  if (CanDrawFace(adjacentChunks, position, BlockFacing::Dir_East)) {
    //FillPackedPositionBuffer(packedPositionBuffer, eastPos, vertexOffset);
    FillPositionBuffer(positionBuffer, eastPos, vertexOffset);
    const BlockQuad east = BlockQuad(positionBuffer, texCoords, eastCols);
    mesh->AddQuad(east);
  }
  if (CanDrawFace(adjacentChunks, position, BlockFacing::Dir_South)) {
    //FillPackedPositionBuffer(packedPositionBuffer, southPos, vertexOffset);
    FillPositionBuffer(positionBuffer, southPos, vertexOffset);
    const BlockQuad south = BlockQuad(positionBuffer, texCoords, southCols);
    mesh->AddQuad(south);
  }
  if (CanDrawFace(adjacentChunks, position, BlockFacing::Dir_West)) {
    //FillPackedPositionBuffer(packedPositionBuffer, westPos, vertexOffset);
    FillPositionBuffer(positionBuffer, westPos, vertexOffset);
    const BlockQuad west = BlockQuad(positionBuffer, texCoords, westCols);
    mesh->AddQuad(west);
  }
  if (CanDrawFace(adjacentChunks, position, BlockFacing::Dir_Up)) {
    //FillPackedPositionBuffer(packedPositionBuffer, topPos, vertexOffset);
    FillPositionBuffer(positionBuffer, topPos, vertexOffset);
    const BlockQuad top = BlockQuad(positionBuffer, texCoords, topCols);
    mesh->AddQuad(top);
  }
}


