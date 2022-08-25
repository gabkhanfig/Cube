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

	static void GenerateQuad1x1(BlockQuad& outQuad, BlockVertexPosition* positions, Color* colors, uint32 textureId) 
	{
		//outQuad.vertices[0] = { positions[0], colors[0], textureId, {0, 0} };
		//outQuad.vertices[1] = { positions[1], colors[1], textureId, {1, 0} };
		//outQuad.vertices[2] = { positions[2], colors[2], textureId, {1, 1} };
		//outQuad.vertices[3] = { positions[3], colors[3], textureId, {0, 1} };
	}
};