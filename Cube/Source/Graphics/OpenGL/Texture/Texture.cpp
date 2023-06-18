#include "Texture.h"
#include <glad/glad.h>

Texture::Texture()
	: textureTarget(0), width(0), height(0)
{
	glGenTextures(1, &id);
}

Texture::~Texture()
{
	glDeleteTextures(1, &id);
}

void Texture::Bind(uint32 slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(textureTarget, id);
}

void Texture::Unbind()
{
	glBindTexture(textureTarget, 0);
}

int Texture::GetDepth() const
{
	return 0;
}

uint8* Texture::LoadPngToBytes(const unsigned char* image, int size, int requiredBitsPerPixel)
{
	stbi_set_flip_vertically_on_load(1);
	int width;
	int height;
	int bitsPerPixel;
	return stbi_load_from_memory(image, size, &width, &height, &bitsPerPixel, requiredBitsPerPixel);
}

void Texture::FlipImageVertically(uint8* imageBytes, int width, int height, int bytesPerPixel)
{
	// Copied from stb_image.h
	int row;
	size_t bytes_per_row = (size_t)width * bytesPerPixel;
	uint8 temp[2048];
	uint8* bytes = (stbi_uc*)imageBytes;

	for (row = 0; row < (height >> 1); row++) {
		uint8* row0 = bytes + row * bytes_per_row;
		uint8* row1 = bytes + (height - row - 1) * bytes_per_row;
		// swap row0 with row1
		size_t bytes_left = bytes_per_row;
		while (bytes_left) {
			size_t bytes_copy = (bytes_left < sizeof(temp)) ? bytes_left : sizeof(temp);
			memcpy(temp, row0, bytes_copy);
			memcpy(row0, row1, bytes_copy);
			memcpy(row1, temp, bytes_copy);
			row0 += bytes_copy;
			row1 += bytes_copy;
			bytes_left -= bytes_copy;
		}
	}
}
