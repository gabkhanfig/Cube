#include "IBlock.h"


BlockMesh IBlock::CreateBlockMesh(Chunk* chunk, WorldPosition position) const
{
  EMeshType meshType = GetMeshType();
  switch (meshType) {
  case EMeshType::cube:
    return CreateCubeMesh(chunk, position);
  case EMeshType::custom:
    return BlockMesh();
  }
  
}

void IBlock::OnDestroy()
{
}

BlockMesh IBlock::CreateCubeMesh(Chunk* chunk, WorldPosition position) const
{
  const EBlockTexture allSideTexture = GetAllSidedTexture();
  const glm::vec2 texCoords[4] = {
    BlockTextureAtlas::GetTextureCoord(allSideTexture, {0, 0}),
    BlockTextureAtlas::GetTextureCoord(allSideTexture, {1, 0}),
    BlockTextureAtlas::GetTextureCoord(allSideTexture, {1, 1}),
    BlockTextureAtlas::GetTextureCoord(allSideTexture, {0, 1})
  };
  
  const glm::vec3 bottomPos[4] = {
   glm::vec3(0, 0, 0),
   glm::vec3(0, 0, 1),
   glm::vec3(1, 0, 1),
   glm::vec3(1, 0, 0)
  };

  const glm::vec3 northPos[4] = {
    glm::vec3(0, 0, 0),
    glm::vec3(1, 0, 0),
    glm::vec3(1, 1, 0),
    glm::vec3(0, 1, 0)
  };

  const glm::vec3 eastPos[4] = {
    glm::vec3(0, 0, 1),
    glm::vec3(0, 0, 0),
    glm::vec3(0, 1, 0),
    glm::vec3(0, 1, 1)
  };

  const glm::vec3 southPos[4] = {
    glm::vec3(1, 0, 1),
    glm::vec3(0, 0, 1),
    glm::vec3(0, 1, 1),
    glm::vec3(1, 1, 1)
  };

  const glm::vec3 westPos[4] = {
    glm::vec3(1, 0, 0),
    glm::vec3(1, 0, 1),
    glm::vec3(1, 1, 1),
    glm::vec3(1, 1, 0)
  };

  const glm::vec3 topPos[4] = {
   glm::vec3(0, 1, 0),
   glm::vec3(1, 1, 0),
   glm::vec3(1, 1, 1),
   glm::vec3(0, 1, 1)
  };
  
  const BlockQuad bottom = BlockQuad(bottomPos, texCoords);
  const BlockQuad north = BlockQuad(northPos, texCoords);
  const BlockQuad east = BlockQuad(eastPos, texCoords);
  const BlockQuad south = BlockQuad(southPos, texCoords);
  const BlockQuad west = BlockQuad(westPos, texCoords);
  const BlockQuad top = BlockQuad(topPos, texCoords);

  BlockMesh mesh;
  mesh.AddQuad(bottom);
  mesh.AddQuad(north);
  mesh.AddQuad(east);
  mesh.AddQuad(south);
  mesh.AddQuad(west);
  mesh.AddQuad(top);

  return mesh;
}

