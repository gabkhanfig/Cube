#pragma once

#include <CubeCore.h>

enum class PlayerMovementMode : uint8 
{
	Normal,
	Flying,
	Free
};

enum class PlayerMovementState : uint8 
{
	Grounded,
	Falling
};