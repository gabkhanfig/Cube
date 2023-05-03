#pragma once

#include "../../Graphics/Geometry/BlockGeometry.h"
#include "BlockFactory.h"
#include "../WorldTransform.h"
#include "BlockTextureAtlas.h"
#include "../../Graphics/Geometry/ChunkMesh.h"
#include "../Chunk/ChunkDataTypes.h"

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

class Block
{
public:

	enum class EMeshType {
		cube,
		custom
	};

	enum class EMeshTransparency {
		solid, 
		transparent,
		custom
	};

	/* Get the name of a block. See BLOCK_BODY() macro. */
	virtual inline GlobalString GetName() const = 0;

	/* Get the class of a block. See BLOCK_BODY() macro. */
	virtual inline BlockClass* GetClass() const = 0;

	/* Ensure setting whatever references to this block to be nullptr, or removed entirely. Prefer this to operator delete for blocks. */
	void Destroy();
	
	/* Create the mesh data for the block, adding to the Chunk's mesh. Must be multithreading safe. 
	DEVELOPER NOTE!!!! Block's are responsible for their own mesh offsets within the chunk. */
	virtual void AddBlockMeshToChunkMesh(ChunkMesh* chunkMesh, Chunk* chunk, WorldPosition position, glm::vec3 vertexOffset) const;

	virtual EMeshTransparency GetFaceTransparency(BlockFacing face) const { return EMeshTransparency::solid; }

	BlockLight GetLight() const { return light; }
	BlockFacing GetFacing() const { return facing; }

	/* Set the lighting value of this block. */
	void SetLight(BlockLight newLight);
	/* Set the facing value of this block. */
	void SetFacing(BlockFacing newFacing);

protected:

	/* Event to be fired upon this block getting destroyed (or not actually destroyed if its using the global block object). */
	virtual void OnDestroy();

	/* Gets the texture that will be used on all sides of this block.  */
	virtual EBlockTexture GetAllSidedTexture() const { return EBlockTexture::none; }

	/**/
	virtual EMeshType GetMeshType() const = 0;

	void CreateCubeMesh(ChunkMesh* chunkMesh, Chunk* chunk, WorldPosition position, glm::vec3 vertexOffset) const;

	bool CanDrawFace(WorldPosition position, BlockFacing face) const;

protected:

	BlockLight light;
	BlockFacing facing;

};