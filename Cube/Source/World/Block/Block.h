#pragma once

#include <EngineCore.h>
#include "../../Graphics/Geometry/BlockGeometry.h"
#include "BlockFactory.h"

/* Sets everything after this back to private. */
#define BLOCK_BODY(blockClass, blockName) \
public:	\
static GlobalString GetStaticName() { \
	return blockName; \
} \
virtual inline GlobalString GetName() { \
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
virtual inline BlockClass* GetClass() { \
	return blockClass::GetStaticClass(); \
} \
private:

class Chunk;

class Block
{
	BLOCK_BODY(Block, "testBlock");

private:

public:

	static bool ShouldCreateNewBlock() { return true; }
};