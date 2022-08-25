#pragma once

#include <CubeCore.h>
#include <glad/glad.h>

enum class TextureTargets : uint32 {
	Invalid = 0,
	Texture2d = GL_TEXTURE_2D,
	Texture2dArray = GL_TEXTURE_2D_ARRAY
};

class Texture 
{
private:

	GLenum textureTarget;

	uint32 rendererId;

	int width;
	int height;

	uint32 texArrayCurrentSize;
	uint32 texArrayMaxLayers;

public:

	Texture(TextureTargets inTextureTarget);
	~Texture();

	/* Bind this texture and an optional specific texture slot. See GL_TEXTUREx (x is any number up to 31, maybe less on some machines). */
	inline void Bind(uint32 slot = 0)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(textureTarget, rendererId);
	}

	/* Unbind this texture. */
	inline void Unbind()
	{
		glBindTexture(textureTarget, 0);
	}

	/* Get the width (in pixels) of the texture / texture array. */
	inline int GetWidth() const { return width; }

	/* Get the height (in pixels) of the texture / texture array. */
	inline int GetHeight() const { return height; }

	/* Get the amount of layers set in texture array. */
	inline uint32 GetTextureArrayCurrentSize() const { return texArrayCurrentSize; }

	/* Get the maximum amount of layers this texture array can hold. */
	inline uint32 GetTextureArrayMaxLayers() const { return texArrayMaxLayers; }

	/* Get the texture target (type of texture) of this texture. */
	inline TextureTargets GetTextureTarget() const { return TextureTargets(textureTarget); }

	static Texture* MakeTexture2d();

	static Texture* MakeTexture2dArray(uint32 layers = 2048, int imageWidth = 16, int imageHeight = 16);

	/* Load an image from a specified path. */
	static uint8* LoadImage(const std::string& imagePath, int* outWidth, int* outHeight, int* outBitsPerPixel);

	/* Frees the memory of the loaded image from Texture::LoadImage(). */
	static void FreeImage(uint8* imageBuffer);

	/* Binds and sets the image of this texture. */
	void SetTexture2dImage(const std::string& imagePath);

	/* Set a layer of the texture 2d array to be a specific image. */
	void SetTexture2dArrayLayer(uint32 layer, uint8* imageData);

	/* Add an image at a specific path to the end of the texture array. */
	uint32 AddTextureToArray(const std::string& imagePath);

	/* Get the maximum amount of texture array layers allowed on the running machine. */
	static int GetMaximumTextureArrayLayersOnMachine();
};