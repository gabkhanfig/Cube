#pragma once

#include "../../EngineCore.h"

class Texture
{

protected:


	uint32 id;

	uint32 textureTarget;

	int width;

	int height;

public:

	Texture();
	~Texture();

	void Bind(uint32 slot = 0);

	void Unbind();

	forceinline int GetWidth() const { return width; }
	forceinline int GetHeight() const { return height; }
	virtual int GetDepth() const;

	static uint8* LoadPngToBytes(const unsigned char* image, int size, int requiredBitsPerPixel);

};

