#include "Texture2d.h"
#include <glad/glad.h>
#include "../../Vendor/stb_image/stb_image.h"

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

Texture2dAtlasData Texture2d::CreateTexture2dAtlas(const darray<const char*>& images, int bytesPerImageFile, int width, int height, int bitsPerPixel)
{
	const int imageCount = images.Size();
	const int imageByteCount = width * height * bitsPerPixel;
	const int atlasWidth = ceil(sqrt(double(imageCount)));
	const int atlasHeight = atlasWidth;
	const int atlasByteCount = atlasWidth * atlasHeight * imageByteCount;

	uint8* buffer = new uint8[atlasByteCount];
	memset(buffer, 0, atlasByteCount);

	darray<uint8*> startLocations;
	for (int y = 0; y < atlasHeight; y++) {
		uint8* vertical = buffer + (y * (2048));
		for (int x = 0; x < atlasWidth; x++) {
			startLocations.Add(vertical + (x * width * bitsPerPixel));
		}
	}

	uint8* start = buffer;

	for (int i = 0; i < imageCount; i++) {
		uint8* imageBuffer = LoadPngToBytes((const unsigned char*)images[i], bytesPerImageFile, bitsPerPixel);
		uint8* writeLocation = startLocations[i];
		uint8* readLocation = imageBuffer;
		for (int h = 0; h < height; h++) {
			memcpy(writeLocation, readLocation, width * bitsPerPixel);
			writeLocation += atlasWidth * width * bitsPerPixel;
			readLocation += width * bitsPerPixel;
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
