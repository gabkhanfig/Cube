#pragma once

#include <CubeCore.h>
#include <Engine/Render/Geometry/Vertex.h>
#include <Game/World/WorldPosition.h>
#include <Game/World/Chunk/Chunk.h>
#include <Engine/Render/Geometry/Quad.h>
#include <Game/World/Block/BlockData.h>
#include <Game/World/Block/Texture/BlockTextures.h>
#include <Game/World/Block/Ids/BlockIds.h>

class Chunk;

class Block 
{
public:

	//BlockPosition localPosition;
	//Chunk* chunk;

	static Block* singleton;
	static uint32 blockId;

public:

	static void SetupSingletonTest();
	static Block* GetSingletonTest();

	virtual inline uint32 GetBlockId() { return UINT_MAX; }
	virtual inline std::string GetBlockName() { return std::string(); }

	virtual inline bool GetIsFaceTransparent(Facing face) {
		return false;
	}

	virtual uint32 GetTextureFaceId(Facing face);

#define COMPRESSED_RENDER

	inline Color GetBlockColor() { return { 255, 255, 255, 255 }; }

	inline float GetBlockFaceBrightness(Facing face)
	{
		constexpr float TopFaceBrightness = 1.f;
		constexpr float NorthSouthFaceBrightness = 0.9f;
		constexpr float EastWestFaceBrightness = 0.8f;
		constexpr float BottomFaceBrightness = 0.76f;
		switch (face) {
		case Facing::Down: return BottomFaceBrightness;
		case Facing::North:	return NorthSouthFaceBrightness;
		case Facing::East:		return EastWestFaceBrightness;
		case Facing::South:	return NorthSouthFaceBrightness;
		case Facing::West:		return EastWestFaceBrightness;
		case Facing::Up:		return TopFaceBrightness;
		}
	}

	inline Color GetFaceColor(Facing face /* , some lighting */)
	{
		const float faceBaseLighting = GetBlockFaceBrightness(face);
		Color blockColor = GetBlockColor();
		Color faceColor;
		faceColor.r = float(blockColor.r) * faceBaseLighting;
		faceColor.g = float(blockColor.g) * faceBaseLighting;
		faceColor.b = float(blockColor.b) * faceBaseLighting;
		faceColor.a = blockColor.a;
		return faceColor;
	}

	virtual bool IsBlockAdjacentTransparent(Chunk* owningChunk, WorldPosition position, Facing face);

	void GenerateQuadDataForFace(BlockPosition position, Facing face, BlockQuad& outQuad);

	virtual bool GenerateBlockQuads(Chunk* chunk, const WorldPosition& worldPos, BlockTransform transform, BlockQuad* quadBuffer, uint32& outQuadsGenerated);

	/* Destroy this block. If it's a simple block, does absolutely nothing. If it's a complex block, actually deletes it. */
	virtual void Destroy();

};