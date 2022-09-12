#pragma once

#include <CubeCore.h>

class Player;

class PlayerInput 
{
	friend class Player;

	static void SetupPlayerInput(Player* player);

	static void MovePlayerForwards(float deltaTime);
	static void MovePlayerBackwards(float deltaTime);
	static void MovePlayerRight(float deltaTime);
	static void MovePlayerLeft(float deltaTime);
	static void MovePlayerUp(float deltaTime);
	static void MovePlayerDown(float deltaTime);
};