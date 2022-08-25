#include "Texture.h"
#include <Vendor/stb_image/stb_image.h>

const std::string defaultPath = "Source/Resources/Textures/";

Texture::Texture(TextureTargets inTextureTarget)
{
  textureTarget = GLenum(inTextureTarget); 
  glGenTextures(1, &rendererId);

  width = 0;
  height = 0; 
  texArrayMaxLayers = 0;
  texArrayCurrentSize = 0;
}

Texture::~Texture()
{
  glDeleteTextures(1, &rendererId);
}

Texture* Texture::MakeTexture2d()
{
  Texture* tex = new Texture(TextureTargets::Texture2d);
  tex->Bind();

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  return tex;
}

Texture* Texture::MakeTexture2dArray(uint32 layers, int imageWidth, int imageHeight)
{
  Texture* tex = new Texture(TextureTargets::Texture2dArray);
  tex->width = imageWidth;
  tex->height = imageHeight;
  tex->texArrayMaxLayers = layers;
  tex->texArrayCurrentSize = 0;

  tex->Bind();

  glTexParameteri(tex->textureTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(tex->textureTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(tex->textureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(tex->textureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGB8, tex->width, tex->height, layers, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

  return tex;
}

uint8* Texture::LoadImage(const std::string& imagePath, int* outWidth, int* outHeight, int* outBitsPerPixel)
{
  stbi_set_flip_vertically_on_load(1);
  return stbi_load(imagePath.c_str(), outWidth, outHeight, outBitsPerPixel, 4);
}

void Texture::FreeImage(uint8* imageBuffer)
{
  if (imageBuffer) {
    stbi_image_free(imageBuffer);
  }
}

void Texture::SetTexture2dImage(const std::string& imagePath)
{
  Bind();
  int bitsPerPixel;
  uint8* imageBuffer = Texture::LoadImage(imagePath, &width, &height, &bitsPerPixel);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBuffer);
  Texture::FreeImage(imageBuffer);
}

void Texture::SetTexture2dArrayLayer(uint32 layer, uint8* imageData)
{
  glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, layer, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
}

uint32 Texture::AddTextureToArray(const std::string& imagePath)
{
  check(texArrayCurrentSize < texArrayMaxLayers);

  int iwidth, iheight, bitsPerPixel;
  uint8* imageBuffer = Texture::LoadImage(imagePath, &iwidth, &iheight, &bitsPerPixel);

  check(width == iwidth);
  check(height == iheight);

  SetTexture2dArrayLayer(texArrayCurrentSize, imageBuffer);
  texArrayCurrentSize++;
  Texture::FreeImage(imageBuffer);
  return texArrayCurrentSize - 1;
}

int Texture::GetMaximumTextureArrayLayersOnMachine()
{
  int maxLayers;
  glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &maxLayers);
  return maxLayers;
}
