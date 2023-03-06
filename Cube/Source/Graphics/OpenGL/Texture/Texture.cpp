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