#pragma once

#include <CubeCore.h>
#include <Engine/Render/Geometry/Vertex.h>
#include <Game/World/WorldPosition.h>
#include <Game/World/Chunk/Chunk.h>
#include <Engine/Render/Geometry/Quad.h>

class Chunk;

enum class BlockFace : uint8
{
	Bottom,
	North,
	East,
	South,
	West,
	Top
};

class Block 
{
public:

	BlockPosition localPosition;
	Chunk* chunk;

public:

	WorldPosition GetWorldPosition();

	void GenerateQuadDataForFace(BlockFace face, Quad& outQuad);

	inline glm::vec4 GetBlockColor() { return { 1.0f, 1.0f, 1.0f, 1.0f }; }

	inline float GetBlockFaceBrightness(BlockFace face) 
	{
		constexpr float TopFaceBrightness					= 1.f;
		constexpr float NorthSouthFaceBrightness	= 0.9f;
		constexpr float EastWestFaceBrightness		= 0.8f;
		constexpr float BottomFaceBrightness			= 0.76f;
		switch (face) {
			case BlockFace::Bottom: return BottomFaceBrightness;
			case BlockFace::North:	return NorthSouthFaceBrightness;
			case BlockFace::East:		return EastWestFaceBrightness;
			case BlockFace::South:	return NorthSouthFaceBrightness;
			case BlockFace::West:		return EastWestFaceBrightness;
			case BlockFace::Top:		return TopFaceBrightness;
		}
	}

	inline glm::vec4 GetFaceColor(BlockFace face, const glm::vec3& faceLighting = {1.0f, 1.0f, 1.0f})
	{
		const float faceBaseLighting = GetBlockFaceBrightness(face);
		const glm::vec4 faceColor = {
			GetBlockColor().r * faceLighting.r * faceBaseLighting,
			GetBlockColor().g * faceLighting.g * faceBaseLighting,
			GetBlockColor().b * faceLighting.b * faceBaseLighting,
			GetBlockColor().a
		};
		return faceColor;
	}

	inline bool GetIsFaceTransparent(BlockFace face) {
		return false;
	}

	inline bool IsBlockAdjacentTransparent(BlockFace face) 
	{
		/*
		+Z -> looking at north
		-Z -> looking at south
		+X -> looking at west
		-X -> looking at east
		+Y -> looking at bottom
		-Y -> looking at top
		*/

		BlockPosition newPosition = localPosition;
		BlockFace opposite = BlockFace::Bottom;
		switch (face) {
		case BlockFace::Bottom:
			newPosition.y--;
			opposite = BlockFace::Top;
			break;
		case BlockFace::North:
			newPosition.z--;
			opposite = BlockFace::South;
			break;
		case BlockFace::East:
			newPosition.x++;
			opposite = BlockFace::West;
			break;
		case BlockFace::South:
			newPosition.z++;
			opposite = BlockFace::North;
			break;
		case BlockFace::West:
			newPosition.x--;
			opposite = BlockFace::East;
			break;
		case BlockFace::Top:
			newPosition.y++;
			opposite = BlockFace::Bottom;
			break;
		}

		Block* adjacentBlock = chunk->GetBlockAtLocalPosition(newPosition);
		//std::cout << localPosition.x << " " << localPosition.y << " " << localPosition.z << " | " << newPosition.x << " " << newPosition.y << " " << newPosition.z << '\n';

		if (!adjacentBlock) {
			//std::cout << "there is a block south this one" << '\n';
			return true;
		}
		else {
			//std::cout << "there is NOT a block south this one" << '\n';
			return adjacentBlock->GetIsFaceTransparent(opposite);
		}
		
	}

};