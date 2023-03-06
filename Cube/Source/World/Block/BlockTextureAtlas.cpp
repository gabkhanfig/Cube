#include "BlockTextureAtlas.h"
#include "../../Cube/Resources/GeneratedFiles.h"
#include "../../Graphics/OpenGL/Texture/Texture2d.h"

/* Generates pairs of const char* and BlockTexture ids. */
static void GetTexturePairs(darray<GeneratedPng>& pngData, darray<EBlockTexture>& ids)
{
#define pair(png, id) \
pngData.Add({png, sizeof(png)}); \
ids.Add(id) 

	pair(generated_InvalidTexture_png, EBlockTexture::none);
	pair(generated_StoneBlock_png, EBlockTexture::stone);
}

/* Simple container for block texture atlas data. */
struct BlockAtlasData
{
	Texture2dAtlasData atlasData;
	std::unordered_map<EBlockTexture, glm::vec2> uvs;
};

/* Map the texture atlas data including absolute uvs. */
static BlockAtlasData MapAtlasData()
{
	constexpr int bytes16x16png = 139;
	constexpr int imageWidth = 16;
	constexpr int imageHeight = 16;
	constexpr int bytesPerPixel = 4;

	darray<GeneratedPng> images;
	darray<EBlockTexture> ids;
	GetTexturePairs(images, ids);

	BlockAtlasData atlas;
	atlas.atlasData = Texture2d::CreateTexture2dAtlas(images, bytes16x16png, imageWidth, imageHeight, bytesPerPixel);

	int idIndex = 0;
	for (float y = 0; y < 1; y += 1.f / ((float)atlas.atlasData.width / (float)imageWidth)) {
		for (float x = 0; x < 1; x += 1.f / ((float)atlas.atlasData.height / (float)imageWidth)) {
			EBlockTexture id = EBlockTexture(0);
			if (idIndex < images.Size()) {
				id = ids[idIndex];
			}
			atlas.uvs.insert({ id, {x, y} });
			idIndex++;
		}
	}
	return atlas;
}

const BlockAtlasData atlasData = MapAtlasData();

std::unordered_map<EBlockTexture, glm::vec2> BlockTextureAtlas::textureUVCoords = atlasData.uvs;
int BlockTextureAtlas::width = atlasData.atlasData.width;
int BlockTextureAtlas::height = atlasData.atlasData.height;
uint8* BlockTextureAtlas::textureBytes = atlasData.atlasData.bytes;
float BlockTextureAtlas::offsetPerBlock = atlasData.atlasData.offset;

Texture* BlockTextureAtlas::texture = nullptr;

void BlockTextureAtlas::CreateBlockTextureAtlasObject()
{
	texture = new Texture2d(textureBytes, width, height);
	delete[] textureBytes;
	textureBytes = nullptr;
}

glm::vec2 BlockTextureAtlas::GetTextureCoord(EBlockTexture texture, const glm::vec2 uv)
{
	const glm::vec2 offset = uv * offsetPerBlock;
	auto found = textureUVCoords.find(texture);
	if (found == textureUVCoords.end()) {
		return glm::vec2() + offset;
	}
	const glm::vec2 zeroCoord = found->second;
	return zeroCoord + offset;
}

void BlockTextureAtlas::Bind(uint32 slot)
{
	texture->Bind(slot);
}
