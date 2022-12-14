#pragma once

#include <Game/Entity/Entity.h>
#include <Game/Player/PlayerMovementMode.h>

class Camera;
class PlayerInput;
class Chunk;

class Player : public Entity 
{
	friend class PlayerInput;
	friend class Camera;

private:

	static Player* player;

	/* The chunk the player is currently in. */
	Chunk* chunkIn;

	float movementSpeed;

	PlayerMovementMode movementMode;
	PlayerMovementState movementState;

public:

	Player();

	inline static Player* GetPlayer() { return player; }

	static Player* StartPlayer();

	virtual void Tick(float deltaTime) override;

	void CheckIfCrossedChunkBorder(const glm::dvec3& oldPos);
	void CrossChunkBorder(Chunk* newChunk);

	void ForwardHold(float deltaTime, float scale);
	void RightHold(float deltaTime, float scale);
	void UpHold(float deltaTime, float scale);


};