#pragma once

#include "../../Block.h"

class AirBlock : public Block 
{
	BLOCK_BODY(AirBlock, "airBlock");

	virtual EMeshType GetMeshType() const override { return EMeshType::custom; }

	virtual EMeshTransparency GetFaceTransparency(BlockFacing face) const { return EMeshTransparency::transparent; }
};