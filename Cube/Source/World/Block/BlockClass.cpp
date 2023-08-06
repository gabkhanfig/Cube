#include "BlockClass.h"

const BlockVTable* IBlockClass::NewBlockVTable() const
{
	BlockVTable* vTable = BlockVTable::Default(this->GetBlockName());
	this->ConstructBlockVTable(vTable);
	return vTable;
}

Block AirBlockClass::ClassDefaultBlock() const
{
	Block air;
	air.isFullSolid = false;
	return air;
}

void AirBlockClass::ConstructBlockVTable(BlockVTable* vTable) const
{
	vTable->meshFunc = AirBlockClass::AirNoMesh;
	vTable->getFaceTexture = AirBlockClass::GetFaceTexture;
}

EBlockTexture AirBlockClass::GetFaceTexture(const Block* self, const BlockFacing face)
{
	return EBlockTexture::none;
}

void AirBlockClass::AirNoMesh(const Block* self, ChunkMesh* mesh, const Chunk* chunk, const WorldPosition position, const glm::vec3 vertexOffset, const MappedAdjacentAndBuriedChunks& adjacentChunks)
{
	return;
}

Block StoneBlockClass::ClassDefaultBlock() const
{
	return Block();
}

void StoneBlockClass::ConstructBlockVTable(BlockVTable* vTable) const
{
	vTable->getFaceTexture = StoneBlockClass::GetFaceTexture;
}

EBlockTexture StoneBlockClass::GetFaceTexture(const Block* self, const BlockFacing face)
{
	return EBlockTexture::stone;
}
