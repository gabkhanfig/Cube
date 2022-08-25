#include "Block.h"
#include <Game/World/Chunk/Chunk.h> 
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>

WorldPosition Block::GetWorldPosition()
{
	return WorldPosition::ToWorldPosition(chunk->position, localPosition);
}

void Block::GenerateQuadDataForFace(BlockFace face, Quad& outQuad)
{
	const float texId = 0;

	const WorldPosition wp = GetWorldPosition();
	const glm::vec3 pos = { float(wp.x), float(wp.y), float(wp.z) };

	const glm::vec3 a = pos;
	const glm::vec3 b = { pos.x + 1.f, pos.y, pos.z };
	const glm::vec3 c = { pos.x + 1.f, pos.y, pos.z + 1.f };
	const glm::vec3 d = { pos.x, pos.y, pos.z + 1.f };
	const glm::vec3 e = { pos.x, pos.y + 1.f, pos.z };
	const glm::vec3 f = { pos.x + 1.f, pos.y + 1.f, pos.z };
	const glm::vec3 g = { pos.x + 1.f, pos.y + 1.f, pos.z + 1.f };
	const glm::vec3 h = { pos.x, pos.y + 1.f, pos.z + 1.f };

	const glm::vec4 faceColor = GetFaceColor(face);

	glm::vec3 positions[4] = {};
	glm::vec4 colors[4] = { faceColor, faceColor, faceColor, faceColor };

	switch (face) {
	case BlockFace::Bottom:
		positions[0] = a;
		positions[1] = b;
		positions[2] = c;
		positions[3] = d;
		Quad::GenerateQuad1x1(outQuad, positions, colors, texId);
		break;
	case BlockFace::North:
		positions[0] = b;
		positions[1] = a;
		positions[2] = e;
		positions[3] = f;
		Quad::GenerateQuad1x1(outQuad, positions, colors, texId);
		break;
	case BlockFace::East:
		positions[0] = c;
		positions[1] = b;
		positions[2] = f;
		positions[3] = g;
		Quad::GenerateQuad1x1(outQuad, positions, colors, texId);
		break;
	case BlockFace::South:
		positions[0] = d;
		positions[1] = c;
		positions[2] = g;
		positions[3] = h;
		Quad::GenerateQuad1x1(outQuad, positions, colors, texId);
		break;
	case BlockFace::West:
		positions[0] = a;
		positions[1] = d;
		positions[2] = h;
		positions[3] = e;
		Quad::GenerateQuad1x1(outQuad, positions, colors, texId);
		break;
	case BlockFace::Top:
		positions[0] = e;
		positions[1] = h;
		positions[2] = g;
		positions[3] = f;
		Quad::GenerateQuad1x1(outQuad, positions, colors, texId);
		break;
	}
}
