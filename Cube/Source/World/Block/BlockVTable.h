#pragma once

#include "../../Engine/EngineCore.h"
#include "../Chunk/MappedAdjacentChunks.h"
#include "../WorldTransform.h"
#include "BlockTextureAtlas.h"

struct Block;
class Chunk;
class ChunkMesh;

void FullCubeMeshFunc(
	const Block* self,
	ChunkMesh* mesh, const
	Chunk* chunk,
	const WorldPosition position,
	const glm::vec3 vertexOffset,
	const MappedAdjacentAndBuriedChunks& adjacentChunks);

struct BlockVTable {
	GlobalString blockName;

	/* Function pointer typedef for a block's static/free function constructs a block's mesh. */
	typedef void(*ConstructMeshFunc)(
		const Block* self, 
		ChunkMesh* mesh, const 
		Chunk* chunk, 
		const WorldPosition position, 
		const glm::vec3 vertexOffset, 
		const MappedAdjacentAndBuriedChunks& adjacentChunks);
	ConstructMeshFunc meshFunc;

	/**/
	typedef EBlockTexture(*FaceTextureFunc)(
		const Block* self,
		const BlockFacing face
	);
	FaceTextureFunc getFaceTexture;

	BlockVTable(GlobalString name);

	static BlockVTable* Default(GlobalString name);

};