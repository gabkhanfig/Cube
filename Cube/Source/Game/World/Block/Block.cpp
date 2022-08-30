#include "Block.h"
#include <Game/World/Chunk/Chunk.h> 
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <Game/World/World.h>
#include <Game/World/Dimension/Dimension.h>

Block* Block::singleton;

void Block::SetupSingletonTest()
{
	singleton = new Block();
}

Block* Block::GetSingletonTest()
{
	return singleton;
}

uint32 Block::GetTextureFaceId(Facing face)
{
	return BlockTexture_Invalid;
}

bool Block::IsBlockAdjacentTransparent(Chunk* owningChunk, WorldPosition position, Facing face) 
{
	/*
	+Z -> looking at north
	-Z -> looking at south
	+X -> looking at west
	-X -> looking at east
	+Y -> looking at bottom
	-Y -> looking at top
	*/
	Facing opposite = Facing::Bottom;
	switch (face) {
	case Facing::Bottom:
		position.y--;
		opposite = Facing::Top;
		break;
	case Facing::North:
		position.z--;
		opposite = Facing::South;
		break;
	case Facing::East:
		position.x++;
		opposite = Facing::West;
		break;
	case Facing::South:
		position.z++;
		opposite = Facing::North;
		break;
	case Facing::West:
		position.x--;
		opposite = Facing::East;
		break;
	case Facing::Top:
		position.y++;
		opposite = Facing::Bottom;
		break;
	}

	Block* adjacentBlock = World::GetBlockAtPosition(position);
	if (adjacentBlock == nullptr) {
		return true;
	}
	else {
		return adjacentBlock->GetIsFaceTransparent(opposite);
	}
}

void Block::GenerateQuadDataForFace(Chunk* owningChunk, BlockPosition position, Facing face, BlockQuad& outQuad)
{
	const uint32 texId = GetTextureFaceId(face);

	const BlockVertexPosition pos = { position.x, position.y, position.z, 0 };
	const BlockVertexPosition a = pos;
	const BlockVertexPosition b = { pos.x + 1, pos.y, pos.z };
	const BlockVertexPosition c = { pos.x + 1, pos.y, pos.z + 1 };
	const BlockVertexPosition d = { pos.x, pos.y, pos.z + 1 };
	const BlockVertexPosition e = { pos.x, pos.y + 1, pos.z };
	const BlockVertexPosition f = { pos.x + 1, pos.y + 1, pos.z };
	const BlockVertexPosition g = { pos.x + 1, pos.y + 1, pos.z + 1 };
	const BlockVertexPosition h = { pos.x, pos.y + 1, pos.z + 1 };

	BlockVertexPosition positions[4] = {};

	const Color faceColor = GetFaceColor(face);
	Color colors[4] = { faceColor, faceColor, faceColor, faceColor };

	switch (face) {
	case Facing::Bottom:
		positions[0] = a;
		positions[1] = b;
		positions[2] = c;
		positions[3] = d;
		break;
	case Facing::North:
		positions[0] = b;
		positions[1] = a;
		positions[2] = e;
		positions[3] = f;
		break;
	case Facing::East:
		positions[0] = c;
		positions[1] = b;
		positions[2] = f;
		positions[3] = g;
		break;
	case Facing::South:
		positions[0] = d;
		positions[1] = c;
		positions[2] = g;
		positions[3] = h;
		break;
	case Facing::West:
		positions[0] = a;
		positions[1] = d;
		positions[2] = h;
		positions[3] = e;
		break;
	case Facing::Top:
		positions[0] = e;
		positions[1] = h;
		positions[2] = g;
		positions[3] = f;
		break;
	}
	BlockQuad::GenerateQuad1x1(outQuad, positions, colors, texId);
}
