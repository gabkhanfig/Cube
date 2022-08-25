#pragma once

#include <CubeCore.h>

struct Vertex 
{
	glm::vec3 position;
	glm::vec4 color;
	glm::vec2 textureUV;
	float textureLayer;

	Vertex(const glm::vec3& inPosition = { 0.f, 0.f, 0.f }, const glm::vec4& inColor = { 1.f, 1.f, 1.f, 1.f }, glm::vec2 inTextureUV = { 0.f, 0.f }, float inTextureLayer = 0) 
	{
		position = inPosition;
		color = inColor;
		textureUV = inTextureUV;
		textureLayer = inTextureLayer;
	}

	static constexpr uint32 GetPositionQuantity() {
		return sizeof(position) / sizeof(float);
	}

	static constexpr uint32 GetColorQuantity() {
		return sizeof(color) / sizeof(float);
	}

	static constexpr uint32 GetTextureUVQuantity() {
		return sizeof(textureUV) / sizeof(float);
	}

	static constexpr uint32 GetTextureLayerQuantity() {
		return sizeof(textureLayer) / sizeof(float);
	}
};

struct BlockVertexPosition 
{
	uint8 x;
	uint8 y;
	uint8 z;
	uint8 textureUV;

	static const uint8 bottomLeftUV = 0x00;
	static const uint8 bottomRightUV = 0x0f;
	static const uint8 topRightUV = 0xff;
	static const uint8 topLeftUV = 0xf0;
};

struct BlockVertex 
{
	BlockVertexPosition position;
	Color color;
	uint32 textureId;

	BlockVertex(BlockVertexPosition inPosition = { 0, 0, 0, 0 }, Color inColor = { 255, 255, 255, 255 }, uint32 inTextureId = 0)
	{
		position = inPosition;
		color = inColor;
		textureId = inTextureId;
	}

	static constexpr uint32 GetPositionQuantity() {
		return 1;
	}

	static constexpr uint32 GetColorQuantity() {
		return 1;
	}

	static constexpr uint32 GetTextureQuantity() {
		return 1;
	}
};