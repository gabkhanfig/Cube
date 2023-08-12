#pragma once

#include "../../Graphics/Geometry/BlockGeometry.h"
#include "../WorldTransform.h"
#include "BlockTextureAtlas.h"
#include "../../Graphics/Geometry/ChunkMesh.h"
#include "../Chunk/ChunkDataTypes.h"
#include "../../Engine/Object.h"
#include "../Chunk/MappedAdjacentChunks.h"
#include "../Chunk/ChunkBlocksBitmask.h"
#include "BlockPathtrace.h"

/* Required macro for all blocks. Sets its class data and name. Also sets everything after this back to private. */
#define BLOCK_BODY(blockClass, blockName) \
public:	\
static GlobalString GetStaticName() { \
	return blockName; \
} \
virtual inline GlobalString GetName() const { \
	return blockClass::GetStaticName(); \
} \
static gk::ClassRef* _GetStaticClassRef() { \
	static gk::ClassRef* classReference = gk::ClassRef::CreateClassReference<blockClass>(#blockClass); \
	return classReference; \
} \
static BlockClass* GetStaticClass() { \
	static BlockClass* staticClass = BlockClass::CreateBlockDependency<blockClass>(); \
	return staticClass; \
} \
virtual inline BlockClass* GetClass() const { \
	return blockClass::GetStaticClass(); \
} \
private:

#define CUBE_MESH(texture) \
public: \
virtual EBlockTexture GetAllSidedTexture() const override { return texture; } \
virtual EMeshType GetMeshType() const override { return EMeshType::cube; } \
private:

class Chunk;

/* Base class for all in-world blocks.
DEVELOPER NOTE: sizeof(Block) will always be a multiple of 8 due to v-table pointer. Pack as much into this as possible. */
//class Block : public IObject
//{
//public:
//
//	enum class EMeshType : uint8 {
//		cube,
//		custom
//	};
//
//	enum class ETransparency : uint8 {
//		solid, 
//		transparent,
//		custom
//	};
//
//	enum class EBuriedTransparency : bool {
//		solid,
//		transparent
//	};
//
//	/* Get the name of a block. See BLOCK_BODY() macro. */
//	virtual inline GlobalString GetName() const = 0;
//
//	/* Get the class of a block. See BLOCK_BODY() macro. */
//	virtual inline BlockClass* GetClass() const = 0;
//	
//	/* Create the mesh data for the block, adding to the Chunk's mesh. Must be multithreading safe. 
//	DEVELOPER NOTE!!!! Block's are responsible for their own mesh offsets within the chunk. */
//	virtual void AddBlockMeshToChunkMesh(ChunkMesh* chunkMesh, Chunk* chunk, WorldPosition position, glm::vec3 vertexOffset) const;
//
//	virtual void AddBlockMeshToChunkMeshBitmaskTest(
//		ChunkMesh* chunkMesh, const Chunk* chunk, const WorldPosition position, const glm::vec3 vertexOffset, const MappedAdjacentAndBuriedChunks& adjacentChunks) const;
//
//	virtual ETransparency GetFaceTransparency(BlockFacing face) const { return ETransparency::solid; }
//	virtual EBuriedTransparency GetBuriedTransparency() const { return EBuriedTransparency::solid; }
//	virtual BlockFacing GetSolidSides() const { return BlockFacing::Dir_Down | BlockFacing::Dir_Up | BlockFacing::Dir_North | BlockFacing::Dir_East | BlockFacing::Dir_South | BlockFacing::Dir_West; }
//
//	BlockLight GetLight() const { return light; }
//	void SetLight(BlockLight newLight);
//
//	BlockFacing GetFacing() const { return facing; }
//	void SetFacing(BlockFacing newFacing);
//
//	bool IsBuried(const Chunk* owningChunk, const MappedAdjacentChunks& adjacentChunks, WorldPosition worldPosition, BlockPosition blockPos) const;
//
//	forceinline bool IsSolid() const { return isSolid; }
//
//#pragma region IObject_Implementation
//
//	virtual bool IsPendingDelete() const final { return isPendingDelete; }
//	virtual void _SetIsPendingDelete() final { isPendingDelete = true; }
//
//#pragma endregion
//
//protected:
//
//	/* Event to be fired upon this block getting destroyed (or not actually destroyed if its using the global block object). */
//	virtual void OnDestroy();
//
//	/* Gets the texture that will be used on all sides of this block.  */
//	virtual EBlockTexture GetAllSidedTexture() const { return EBlockTexture::none; }
//
//	/**/
//	virtual EMeshType GetMeshType() const = 0;
//
//	void CreateCubeMesh(ChunkMesh* chunkMesh, Chunk* chunk, WorldPosition position, glm::vec3 vertexOffset) const;
//
//	bool CanDrawFace(WorldPosition position, BlockFacing face) const;
//
//	bool CanDrawFace(const MappedAdjacentAndBuriedChunks& adjacentChunks, WorldPosition position, BlockFacing face) const;
//
//	bool CanDrawFaceWithinChunk(const Chunk* chunk, const WorldPosition position, const BlockFacing face) const;
//
//protected:
//
//	/* The lighting of this block. */
//	BlockLight light;
//
//	/* The 6 facing direction of this block. Not necessarily rotation. Cast to BlockFacing. */
//	uint8 facing : 6;
//	
//	/* IObject pending delete flag. */
//	uint8 isPendingDelete : 1;
//
//	/* Remaining bit for potential future use. */
//	uint8 isSolid : 1;
//
//	// 4 bytes remaining for 16 byte padding.
//
//};

struct BlockVTable;

struct Block
{
	const BlockVTable* vTable;
	void* _emptyFutureBlockDataComponent;
	BlockLight light;
	uint8 facing : 6;
	uint8 isPendingDelete : 1;
	uint8 isFullSolid : 1;
	// 4 more bytes for 24 byte size

	Block();
	/* Will not free any memory. Use BlockTest::Destroy() instead. */
	~Block();

	Block(const Block& other);
	Block& operator =(const Block& other);

	/**/
	void Destroy();

	GlobalString GetName() const;

	bool IsBuried(const Chunk* owningChunk, const MappedAdjacentChunks& adjacentChunks, WorldPosition worldPosition, BlockPosition blockPos) const;

	void ConstructMesh(ChunkMesh* mesh, const Chunk* chunk, const WorldPosition position, const glm::vec3 vertexOffset, const MappedAdjacentAndBuriedChunks& adjacentChunks) const;

	CompressedBlockPathtraceData GetPathtraceData() const;
};