#pragma once

#include "../../Engine/EngineCore.h"
#include "../WorldTransform.h"
#include "../Chunk/MappedAdjacentChunks.h"
#include "BlockTextureAtlas.h"

template<typename T> struct is_function : public std::false_type {};
template<typename Ret, typename ...Args> struct is_function<Ret(*)(Args...)> : public std::true_type {};
template<typename Fn, typename FnSignature> concept _BlockClassImplFunc = is_function<Fn>::value && std::is_same_v<Fn, FnSignature>;

struct Block;
class Chunk;
class ChunkMesh;
struct BlockVTable;

template<typename T>
concept BlockClassImplementsVtable =
_BlockClassImplFunc<decltype(&T::constructMesh), gk::Fptr<void,
	const Block*,
	const BlockVTable*,
	ChunkMesh*,
	const Chunk*,
	const WorldPosition&,
	const glm::vec3&,
	const MappedAdjacentAndBuriedChunks&>::FuncPtrT>
	&& _BlockClassImplFunc<decltype(&T::getFaceTexture), gk::Fptr<EBlockTexture, const Block*, BlockFacing>::FuncPtrT>;



struct BlockVTable 
{
	gk::Fptr<void,
	const Block*,
	const BlockVTable*,
	ChunkMesh*,
	const Chunk*,
	const WorldPosition&,
	const glm::vec3&,
	const MappedAdjacentAndBuriedChunks&> 
		mesh;

	gk::Fptr<EBlockTexture, const Block*, BlockFacing> faceTexture;

	/* Contains some default functions */
	struct Default {
		static void FullCubeMeshFunc(
			const Block* self,
			const BlockVTable* vTable,
			ChunkMesh* mesh,
			const Chunk* chunk,
			const WorldPosition& position,
			const glm::vec3& vertexOffset,
			const MappedAdjacentAndBuriedChunks& adjacentChunks
		);
	};

};

/* Shared between all instances of a block */
struct BlockTypeInfo
{
	GlobalString blockName;
	bool usesDataComponent;
	bool usesFacing;
	bool isFullSolid;
	BlockVTable vTable;
};
