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

BlockMaterial AirBlockClass::GetMaterial() const
{
	gk_assertm(false, "Air Block does not have a material");
	return BlockMaterial();
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

BlockMaterial StoneBlockClass::GetMaterial() const
{
	BlockMaterial material;
	material.model = EBlockModel::Cube;
	material.faceTextures.bottomFaceTexture = EBlockTexture::stone;
	material.faceTextures.northFaceTexture = EBlockTexture::stone;
	material.faceTextures.eastFaceTexture = EBlockTexture::stone;
	material.faceTextures.southFaceTexture = EBlockTexture::stone;
	material.faceTextures.westFaceTexture = EBlockTexture::stone;
	material.faceTextures.topFaceTexture = EBlockTexture::stone;
	return material;
}

EBlockTexture StoneBlockClass::GetFaceTexture(const Block* self, const BlockFacing face)
{
	return EBlockTexture::stone;
}
