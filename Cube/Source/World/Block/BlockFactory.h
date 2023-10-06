#pragma once

#include "../../Engine/EngineCore.h"
#include "BlockClass.h"
#include "Block.h"
#include "BlockTypeInfo.h"
#include "../../Graphics/Geometry/Material.h"

class Chunk;
struct BlockVTable;

struct BlockFactory
{
public:

	template<typename T> 
		requires (std::is_base_of<IBlockClass, T>::value && BlockClassImplementsVtable<T>)
	static void AddBlock() {
		const IBlockClass* blockClass = new T();

		BlockTypeInfo typeInfo;
		typeInfo.blockName = blockClass->getBlockName();
		typeInfo.isFullSolid = blockClass->isFullSolid();
		typeInfo.usesDataComponent = false;
		typeInfo.usesFacing = false;

		typeInfo.vTable.mesh = T::constructMesh;
		typeInfo.vTable.faceTexture = T::getFaceTexture;

		const uint32 blockId = allBlockTypeInfos.Size();

		if constexpr (std::is_same_v<T, AirBlockClass>) {
			gk_assertm(blockId == 0, "Air Block Type Info must be first in the array");
		}

		allBlockTypeInfos.Add(typeInfo);
		blockIds.insert({ typeInfo.blockName, blockId });
		delete blockClass;
	}

	static bool IsValidBlockName(const GlobalString blockName);

	static Block CreateBlock(const GlobalString blockName);
	static Block createBlock(const uint32 id);

	static Block CreateAirBlock();

	static const BlockTypeInfo* getBlockTypeInfo(const GlobalString blockName);
	
	static const BlockTypeInfo* getBlockTypeInfo(const uint32 id);

private:

	//static std::unordered_map<GlobalString, const BlockConstructionPair*> blockClasses;
	//static std::unordered_map<GlobalString, uint32> materialIds;
	//static darray<Material> materialsData;
	//static const BlockConstructionPair* airFactory;

	static darray<BlockTypeInfo> allBlockTypeInfos;
	static std::unordered_map<GlobalString, uint32> blockIds;



};
