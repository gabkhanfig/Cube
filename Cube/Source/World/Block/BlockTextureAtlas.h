#pragma once

#include <EngineCore.h>

enum class EBlockTexture
{
	none,
	wood,
	black,
	blue
};

class Texture;

/* Texture atlas for blocks. Loaded with application. See CreateBlockTextureAtlasObject() for actually creating the opengl texture object. */
class BlockTextureAtlas
{
	static std::unordered_map<EBlockTexture, glm::vec2> textureUVCoords;

	static int width;

	static int height;

	static uint8* textureBytes;

	static float offsetPerBlock;

	static Texture* texture;

public:

	/* Load the block texture atlas data into an OpenGL texture. */
	static void CreateBlockTextureAtlasObject();

	/* Get the texture coordinate of a texture id along with the relative uv offset. 
	Example coordinates (1, 1) would translate to (0.5, 0.5) in a 2x2 texture atlas targetting the bottom left texture. 
	@param texture: The block texture id.
	@param uv: The relative uv coordinates from (0, 0) to (1, 1) that will be transformed. (0, 0) is bottom left. 
	@return Absolute uv coordinates. */
	static glm::vec2 GetTextureCoord(EBlockTexture texture, const glm::vec2 uv);

	static void Bind(uint32 slot);

};