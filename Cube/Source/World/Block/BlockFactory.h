#pragma once

#include "../../Engine/EngineCore.h"
#include "BlockClass.h"
#include "Block.h"
#include "../../Graphics/Geometry/Material.h"

class Chunk;
struct BlockVTable;

struct BlockFactory
{
public:

	template<typename T> 
		requires (std::is_base_of<IBlockClass, T>::value)
	static void AddBlock() {
		const IBlockClass* blockClass = new T();
		const BlockVTable* vTable = blockClass->NewBlockVTable();

#if CUBE_DEVELOPMENT
		AssertValidateVTable(blockClass->GetBlockName(), vTable);
#endif

		BlockConstructionPair* pair = new BlockConstructionPair();
		pair->blockClass = blockClass;
		pair->vTable = vTable;
		blockClasses.insert({ blockClass->GetBlockName(), pair });

		if constexpr (std::is_same_v<T, AirBlockClass>) {
			airFactory = pair;
		}
		else {
			const Material material = Material::ConvertFromBlockMaterial(blockClass->GetMaterial());
			const uint32 index = materialsData.Add(material);
			materialIds.insert({ blockClass->GetBlockName(), index });
		}
	}

	static bool IsValidBlockName(const GlobalString blockName);

	static Block CreateBlock(const GlobalString blockName);

	static Block CreateAirBlock();

	static const IBlockClass* GetBlockClass(const GlobalString blockName);

	static uint32 GetMaterialId(const GlobalString blockName);

private:

	static void AssertValidateVTable(GlobalString blockName, const BlockVTable* vTable);

private:

	static std::unordered_map<GlobalString, const BlockConstructionPair*> blockClasses;
	static std::unordered_map<GlobalString, uint32> materialIds;
	static darray<Material> materialsData;
	static const BlockConstructionPair* airFactory;

};
