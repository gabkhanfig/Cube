#pragma once

#include "Vertex.h"

struct Quad 
{
	Vertex vertices[4];

	/* Counter clockwise quad, using 1x1 texture uvs, starting at bottom left relative. */
	static void GenerateQuad1x1(Quad& outQuad, glm::vec3* positions, glm::vec4* colors, float textureLayer) 
	{
		constexpr glm::vec2 p1UV = { 0.f, 0.f };
		constexpr glm::vec2 p2UV = { 1.f, 0.f };
		constexpr glm::vec2 p3UV = { 1.f, 1.f };
		constexpr glm::vec2 p4UV = { 0.f, 1.f };

		outQuad.vertices[0] = { positions[0], colors[0], p1UV, textureLayer };
		outQuad.vertices[1] = { positions[1], colors[1], p2UV, textureLayer };
		outQuad.vertices[2] = { positions[2], colors[2], p3UV, textureLayer };
		outQuad.vertices[3] = { positions[3], colors[3], p4UV, textureLayer };
	}
};

struct BlockQuad 
{
	BlockVertex vertices[4];

	/* Counter clockwise quad, using 1x1 texture uvs, starting at bottom left relative. */
	static void GenerateQuad1x1(BlockQuad& outQuad, BlockVertexPosition positions[4], Color colors[4], uint32 textureId)
	{
		constexpr uint32 bottomLeftUV = (0U << 30) + (0U << 31);
		constexpr uint32 bottomRightUV = (1U << 30) + (0U << 31);
		constexpr uint32 topRightUV = (1U << 30) + (1U << 31);
		constexpr uint32 topLeftUV = (0U << 30) + (1U << 31);

		outQuad.vertices[0] = { positions[0], colors[0], bottomLeftUV | textureId };
		outQuad.vertices[1] = { positions[1], colors[1], bottomRightUV | textureId };
		outQuad.vertices[2] = { positions[2], colors[2], topRightUV | textureId };
		outQuad.vertices[3] = { positions[3], colors[3], topLeftUV | textureId };
	}
};