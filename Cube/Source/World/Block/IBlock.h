#pragma once

#include <EngineCore.h>
#include "../../Graphics/Geometry/BlockGeometry.h"
#include "BlockFactory.h"
#include "../WorldTransform.h"
#include "BlockTextureAtlas.h"

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
virtual inline void Destroy() override { \
	OnDestroy(); \
	if (blockClass::ShouldCreateNewBlock()) delete this; \
} \
private:

/* Macro to allow this block to create new versions of itself for chunks. 
If this is not set, the default functionality of using the global block instance will be used. */
#define NEW_BLOCK() \
public: \
static bool ShouldCreateNewBlock() { return true; } \
private:

class Chunk;

class IBlock
{
public:

	static bool ShouldCreateNewBlock() { return false; }

	/* Get the name of a block. See BLOCK_BODY() macro. */
	virtual inline GlobalString GetName() const = 0;

	/* Get the class of a block. See BLOCK_BODY() macro. */
	virtual inline BlockClass* GetClass() const = 0;

	/* Ensure setting whatever references to this block to be nullptr, or removed entirely. Prefer this to operator delete for blocks. */
	virtual inline void Destroy() = 0;
	
	//virtual darray<BlockQuad> GetBlockMesh(Chunk* chunk, WorldPosition position);

protected:

	/* Event to be fired upon this block getting destroyed (or not actually destroyed if its using the global block object). */
	virtual void OnDestroy();

	/* Gets the texture that will be used on all sides of this block.  */
	//virtual EBlockTexture GetAllSidedTexture();

};