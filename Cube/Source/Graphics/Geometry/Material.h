#pragma once

#include "../../Engine/EngineCore.h"
#include "../../World/Block/BlockPathtrace.h"

struct Material
{
	uint32 modelId;
	glm::vec2 bottomFaceTextureCoords;
	glm::vec2 northFaceTextureCoords;
	glm::vec2 eastFaceTextureCoords;
	glm::vec2 southFaceTextureCoords;
	glm::vec2 westFaceTextureCoords;
	glm::vec2 topFaceTextureCoords;

	static Material ConvertFromBlockMaterial(const BlockMaterial blockMaterial);
};