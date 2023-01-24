#pragma once

#include <EngineCore.h>

enum class BlockTexture
{
	wood,
	black,
	blue
};

class Texture;

class BlockTextureAtlas
{
	static std::unordered_map<BlockTexture, glm::vec2> textureUVCoords;

	static int width;

	static int height;

	static uint8* textureBytes;

	static float offsetPerBlock;

	static Texture* texture;

public:

	/* Load the block texture atlas data into an OpenGL texture. */
	static void LoadBlockAtlasTexture();

	/* Get the texture coordinate of a texture id along with the relative uv offset. 
	Example coordinates (1, 1) would translate to (0.5, 0.5) in a 2x2 texture atlas targetting the bottom left texture. 
	@param texture: The block texture id.
	@param uv: The relative uv coordinates from (0, 0) to (1, 1) that will be transformed. (0, 0) is bottom left. 
	@return Absolute uv coordinates. */
	static glm::vec2 GetTextureCoord(BlockTexture texture, const glm::vec2 uv);

};