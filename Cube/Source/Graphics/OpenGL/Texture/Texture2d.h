#pragma once

#include "Texture.h"

/* Bytes need to be manually freed. */
struct Texture2dAtlasData
{
	unsigned char* bytes;
	int width;
	int height;
	int imageWidth;
	int imageHeight;
	float offset; 
};

class Texture2d : public Texture
{
private:

	int width;
	int height;

public:

	Texture2d(const unsigned char* imageBytes, int _width, int _height);

	static Texture2dAtlasData CreateTexture2dAtlas(const darray<GeneratedPng>& images, int bytesPerImageFile, int width, int height, int bitsPerPixel);
};