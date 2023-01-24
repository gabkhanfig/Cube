#include "BlockTextureAtlas.h"
#include "../../Cube/Resources/GeneratedFiles.h"
#include <Graphics/Texture/Texture2d.h>

/* Generates pairs of const char* and BlockTexture ids. */
static void GetTexturePairs(darray<const char*>& pngData, darray<BlockTexture>& ids)
{
#define pair(png, id) \
pngData.Add(png); \
ids.Add(id) 

	pair(generated_16x16wood_png, BlockTexture::wood);
	pair(generated_16x16black_png, BlockTexture::black);
	pair(generated_16x16blue_png, BlockTexture::blue);
}

/* Simple container for block texture atlas data. */
struct BlockAtlasData
{
	Texture2dAtlasData atlasData;
	std::unordered_map<BlockTexture, glm::vec2> uvs;
};

/* Map the texture atlas data including absolute uvs. */
static BlockAtlasData MapAtlasData()
{
	constexpr int bytes16x16png = 139;
	constexpr int imageWidth = 16;
	constexpr int imageHeight = 16;
	constexpr int bytesPerPixel = 4;

	darray<const char*> images;
	darray<BlockTexture> ids;
	GetTexturePairs(images, ids);

	BlockAtlasData atlas;
	atlas.atlasData = Texture2d::CreateTexture2dAtlas(images, bytes16x16png, imageWidth, imageHeight, bytesPerPixel);

	int idIndex = 0;
	for (float y = 0; y < 1; y += 1.f / ((float)atlas.atlasData.width / (float)imageWidth)) {
		for (float x = 0; x < 1; x += 1.f / ((float)atlas.atlasData.height / (float)imageWidth)) {
			BlockTexture id = BlockTexture(0);
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

std::unordered_map<BlockTexture, glm::vec2> BlockTextureAtlas::textureUVCoords = atlasData.uvs;
int BlockTextureAtlas::width = atlasData.atlasData.width;
int BlockTextureAtlas::height = atlasData.atlasData.height;
uint8* BlockTextureAtlas::textureBytes = atlasData.atlasData.bytes;
float BlockTextureAtlas::offsetPerBlock = atlasData.atlasData.offset;

Texture* BlockTextureAtlas::texture = nullptr;

void BlockTextureAtlas::LoadBlockAtlasTexture()
{
	texture = new Texture2d(textureBytes, width, height);
	delete[] textureBytes;
	textureBytes = nullptr;
}

glm::vec2 BlockTextureAtlas::GetTextureCoord(BlockTexture texture, const glm::vec2 uv)
{
	auto found = textureUVCoords.find(texture);
	if (found == textureUVCoords.end()) {
		return glm::vec2();
	}
	const glm::vec2 zeroCoord = found->second;
	return zeroCoord + (uv * offsetPerBlock);
}
