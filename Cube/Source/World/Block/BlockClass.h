#pragma once

#include "../../Engine/EngineCore.h"
#include "Block.h"
#include "../WorldTransform.h"
#include "BlockPathtrace.h"

class IBlockClass;

struct BlockConstructionPair
{
	const IBlockClass* blockClass;
	const BlockVTable* vTable;
};

class IBlockClass {
public:

	virtual GlobalString getBlockName() const = 0;
	virtual bool isFullSolid() const = 0;
};


class StoneBlockClass : public IBlockClass
{
public:
	virtual GlobalString getBlockName() const override { return "stone"; }
	virtual bool isFullSolid() const override { return true; }

	static void constructMesh(const Block* self,
		const BlockVTable* vTable,
		ChunkMesh* mesh,
		const Chunk* chunk,
		const WorldPosition& position,
		const glm::vec3& vertexOffset,
		const MappedAdjacentAndBuriedChunks& adjacentChunks);
	static EBlockTexture getFaceTexture(const Block* self, const BlockFacing face);

};

class AirBlockClass : public IBlockClass
{
public:
	virtual GlobalString getBlockName() const override { return "air"; }
	virtual bool isFullSolid() const override { return false; }

	static void constructMesh(const Block* self,
		const BlockVTable* vTable,
		ChunkMesh* mesh,
		const Chunk* chunk,
		const WorldPosition& position,
		const glm::vec3& vertexOffset,
		const MappedAdjacentAndBuriedChunks& adjacentChunks);
	static EBlockTexture getFaceTexture(const Block* self, const BlockFacing face);
	

};