#pragma once

#include "BlockTextureAtlas.h"
#include "../../Engine/EngineCore.h"
#include "../../Graphics/Geometry/PackedTypes.h"

enum class EBlockModel {
	Cube = 0
};

struct BlockFaceTextures {
	EBlockTexture bottomFaceTexture;
	EBlockTexture northFaceTexture;
	EBlockTexture eastFaceTexture;
	EBlockTexture southFaceTexture;
	EBlockTexture westFaceTexture;
	EBlockTexture topFaceTexture;
};

struct BlockMaterial
{
	EBlockModel model;
	BlockFaceTextures faceTextures;
};

struct CompressedBlockPathtraceData
{
	uint32 materialId;
	PackedNormal normal;
};