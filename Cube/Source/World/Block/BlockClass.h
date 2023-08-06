#pragma once

#include "../../Engine/EngineCore.h"
#include "BlockVTable.h"
#include "Block.h"
#include "../WorldTransform.h"

class IBlockClass;

//template<typename T>
//concept BlockImplementsName =
//	requires(T) {
//		{ T::GetStaticName() } -> std::same_as<GlobalString>;
//	};

//#define BLOCK_CLASS(name)\
//public:\
//static GlobalString GetStaticName() {return name;}

struct BlockCreatorPair
{
	const IBlockClass* blockClass;
	const BlockVTable* vTable;
};

class IBlockClass {
public:

	const BlockVTable* NewBlockVTable() const;

	virtual GlobalString GetBlockName() const = 0;
	/* DO NOT set the v table. */
	virtual Block ClassDefaultBlock() const = 0;

	virtual void ConstructBlockVTable(BlockVTable* vTable) const = 0;
};


class StoneBlockClass : public IBlockClass
{
	virtual GlobalString GetBlockName() const override { return "stone"; }
	virtual Block ClassDefaultBlock() const override;
	virtual void ConstructBlockVTable(BlockVTable* vTable) const override;

	static EBlockTexture GetFaceTexture(const Block* self, const BlockFacing face);
};

class AirBlockClass : public IBlockClass
{
	virtual GlobalString GetBlockName() const override { return "air"; }
	virtual Block ClassDefaultBlock() const override;
	virtual void ConstructBlockVTable(BlockVTable* vTable) const override;

	static EBlockTexture GetFaceTexture(const Block* self, const BlockFacing face);

	static void AirNoMesh(
		const Block* self,
		ChunkMesh* mesh, const
		Chunk* chunk,
		const WorldPosition position,
		const glm::vec3 vertexOffset,
		const MappedAdjacentAndBuriedChunks& adjacentChunks);
};