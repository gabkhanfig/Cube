#include "Texture2d.h"
#include <glad/glad.h>
#include "../../../Engine/Vendor/stb_image/stb_image.h"

Texture2d::Texture2d(const unsigned char* imageBytes, int _width, int _height)
{
	textureTarget = GL_TEXTURE_2D;
	width = _width;
	height = _height;
	Bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBytes);
}

Texture2dAtlasData Texture2d::CreateTexture2dAtlas(const darray<const CompileTimeFiles::Image*>& images, int bytesPerImageFile, int width, int height, int bytesPerPixel)
{
	const int imageCount = images.Size();																			// Total number of images
	const int imageByteCount = width * height * bytesPerPixel;									// Amount of bytes per image. For 16x16 images, this is 1024
	const int atlasWidth = static_cast<int>(ceil(sqrt(double(imageCount))));	// Width of the texture atlas where each unit quantity is 1 image
	const int atlasHeight = atlasWidth;																				// Height of the texture atlas where each unit quantity is 1 image
	const int atlasByteCount = atlasWidth * atlasHeight * imageByteCount;			// Total bytes used by the texture atlas
	const int bytesBetweenVertical = atlasWidth * imageByteCount;							// Number of bytes to jump to go to the next row of texture atlas

	uint8* buffer = new uint8[atlasByteCount];	// Create a buffer for all of the pixels in the texture atlas.
	memset(buffer, 0, atlasByteCount);					// Initialize it all to 0 (black)

	darray<uint8*> startLocations;
	for (int y = 0; y < atlasHeight; y++) {
		uint8* vertical = buffer + (y * bytesBetweenVertical);
		for (int x = 0; x < atlasWidth; x++) {
			startLocations.Add(vertical + (x * width * bytesPerPixel));
		}
	}

	for (int i = 0; i < imageCount; i++) {
		const CompileTimeFiles::Image* imageFile = images[i];
		uint8* imageBuffer = new uint8[imageFile->totalBytes];
		memcpy(imageBuffer, imageFile->contents, imageFile->totalBytes);
		Texture::FlipImageVertically(imageBuffer, imageFile->width, imageFile->height, bytesPerPixel);

		uint8* writeLocation = startLocations[i];
		const uint8* readLocation = imageBuffer;
		for (int h = 0; h < height; h++) {
			memcpy(writeLocation, readLocation, width * bytesPerPixel);
			writeLocation += atlasWidth * width * bytesPerPixel;
			readLocation += width * bytesPerPixel;
		}
		delete[] imageBuffer;
	}

	Texture2dAtlasData atlasData;
	atlasData.bytes = buffer;
	atlasData.width = atlasWidth * width;
	atlasData.height = atlasHeight * height;
	atlasData.offset = 1.f / (float)atlasWidth;

	return atlasData;
}


