#include "Material.h"
#include "../../World/Block/BlockFactory.h"
#include "../../World/Block/BlockTextureAtlas.h"

Material Material::ConvertFromBlockMaterial(const BlockMaterial blockMaterial)
{
	Material material;
	material.modelId = static_cast<uint32>(blockMaterial.model);
	material.bottomFaceTextureCoords = BlockTextureAtlas::GetTextureCoord(blockMaterial.faceTextures.bottomFaceTexture, glm::vec2(0, 0));
	material.northFaceTextureCoords = BlockTextureAtlas::GetTextureCoord(blockMaterial.faceTextures.northFaceTexture, glm::vec2(0, 0));
	material.eastFaceTextureCoords = BlockTextureAtlas::GetTextureCoord(blockMaterial.faceTextures.eastFaceTexture, glm::vec2(0, 0));
	material.southFaceTextureCoords = BlockTextureAtlas::GetTextureCoord(blockMaterial.faceTextures.southFaceTexture, glm::vec2(0, 0));
	material.westFaceTextureCoords = BlockTextureAtlas::GetTextureCoord(blockMaterial.faceTextures.westFaceTexture, glm::vec2(0, 0));
	material.topFaceTextureCoords = BlockTextureAtlas::GetTextureCoord(blockMaterial.faceTextures.topFaceTexture, glm::vec2(0, 0));
	return material;
}
