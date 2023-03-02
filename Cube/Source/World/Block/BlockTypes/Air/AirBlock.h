#pragma once

#include "../../IBlock.h"

class AirBlock : public IBlock 
{
	BLOCK_BODY(AirBlock, "airBlock");

	virtual EMeshType GetMeshType() const override { return EMeshType::custom; }
};