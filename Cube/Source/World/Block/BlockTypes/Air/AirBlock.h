#pragma once

#include "../../IBlock.h"

class AirBlock : public IBlock 
{
	BLOCK_BODY(AirBlock, "airBlock");

	virtual BlockMesh CreateBlockMesh(Chunk* chunk, WorldPosition position) const override;

	virtual EMeshType GetMeshType() const override { return EMeshType::custom; }
};