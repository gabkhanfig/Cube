#include "BlockClass.h"
#include "BlockTypeInfo.h"

EBlockTexture AirBlockClass::getFaceTexture(const Block* self, const BlockFacing face)
{
	return EBlockTexture::none;
}

void AirBlockClass::constructMesh(const Block* self, const BlockVTable* vTable, ChunkMesh* mesh, const Chunk* chunk, const WorldPosition& position, const glm::vec3& vertexOffset, const MappedAdjacentAndBuriedChunks& adjacentChunks)
{
	return;
}

void StoneBlockClass::constructMesh(const Block* self, const BlockVTable* vTable, ChunkMesh* mesh, const Chunk* chunk, const WorldPosition& position, const glm::vec3& vertexOffset, const MappedAdjacentAndBuriedChunks& adjacentChunks)
{
	return BlockVTable::Default::FullCubeMeshFunc(self, vTable, mesh, chunk, position, vertexOffset, adjacentChunks);
}

EBlockTexture StoneBlockClass::getFaceTexture(const Block* self, const BlockFacing face)
{
	return EBlockTexture::stone;
}
