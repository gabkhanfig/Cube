#pragma once

#include "../../Block.h"

class AirBlock : public Block 
{
	BLOCK_BODY(AirBlock, "airBlock");

public:

	AirBlock();

	virtual EMeshType GetMeshType() const override { return EMeshType::custom; }

	virtual ETransparency GetFaceTransparency(BlockFacing face) const { return ETransparency::transparent; }

	virtual EBuriedTransparency GetBuriedTransparency() const { return EBuriedTransparency::transparent; }
};