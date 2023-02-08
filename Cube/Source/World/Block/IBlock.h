#pragma once

#include <EngineCore.h>
#include "../../Graphics/Geometry/BlockGeometry.h"
#include "BlockFactory.h"

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

	/* Get the name of a block. */
	virtual inline GlobalString GetName() const = 0;

	/* Get the class of a block. */
	virtual inline BlockClass* GetClass() const = 0;
	
	static bool ShouldCreateNewBlock() { return false; }
};