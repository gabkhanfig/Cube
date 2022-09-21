#include "Player.h"
#include <Game/Player/PlayerInput.h>
#include <glm/gtx/rotate_vector.hpp>
#include <Game/World/WorldPosition.h>
#include <Game/World/World.h>
#include <Game/World/Dimension/Dimension.h>
#include <Game/World/Chunk/Chunk.h>

constexpr glm::vec3 upVector = glm::dvec3(0.0, 1.0, 0.0);

Player* Player::player;

Player::Player()
{
	position = { -1.0, 17.0, -1.0 };
	rotation = { 0.0, 0.0, 0.0 };
	velocity = { 0.0, 0.0, 0.0 };

	tickEnabled = true;

	movementMode = PlayerMovementMode::Flying;
	movementState = PlayerMovementState::Grounded;

	movementSpeed = 40;
}

Player* Player::StartPlayer()
{
	player = new Player();
	PlayerInput::SetupPlayerInput(player);
	return player;
}

void Player::Tick(float deltaTime)
{
	

	//float forwardScale = 0;
	//float rightScale = 0;
	//float leftScale = 0;

	Entity::Tick(deltaTime);

	CheckIfCrossedChunkBorder(previousPosition);
	previousPosition = position;

	//velocity = { 0, 0, 0 };
}

void Player::CheckIfCrossedChunkBorder(const glm::dvec3& oldPos)
{
	ChunkPosition current = { floor((position / 16.0).x), floor((position / 16.0).y), floor((position / 16.0).z) };
	ChunkPosition old = { floor((oldPos / 16.0).x), floor((oldPos / 16.0).y), floor((oldPos / 16.0).z) };

	if (current.x != old.x || current.y != old.y || current.z != old.z) {
		chunkIn = World::loadedDimensions[0]->GetChunk(current);
		//Print("crossed chunk border");
		//CrossChunkBorder(chunkIn);
	}
}

void Player::CrossChunkBorder(Chunk* newChunk)
{
	World::loadedDimensions[0]->RemoveDistantChunks();
	World::loadedDimensions[0]->LoadChunksAroundPlayer();
}

void Player::ForwardHold(float deltaTime, float scale)
{
	switch (GetPlayer()->movementMode) {
	case PlayerMovementMode::Free:
		position += deltaTime * scale * GetPlayer()->movementSpeed * GetPlayer()->rotation;
		break;
	default:
		glm::vec3 _rotation = glm::rotate(rotation, glm::radians(90.f), upVector);
		position += deltaTime * scale * movementSpeed * glm::normalize(glm::cross(_rotation, upVector));
		break;
	}
}

void Player::RightHold(float deltaTime, float scale)
{
	position += deltaTime * scale * movementSpeed * glm::normalize(glm::cross(rotation, upVector));
}

void Player::UpHold(float deltaTime, float scale)
{
	if (GetPlayer()->movementMode != PlayerMovementMode::Normal) {
		position.y += deltaTime * scale * GetPlayer()->movementSpeed;
	}
}
