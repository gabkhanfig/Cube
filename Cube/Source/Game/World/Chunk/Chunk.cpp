#include "Chunk.h"
#include <Game/World/World.h>
#include <Game/World/Block/Block.h>
#include <Util/Benchmark/Benchmark.h>
#include <Game/World/Block/Factory/BlockFactory.h>
#include <Game/Player/Player.h>
#include <Engine/Render/Camera/Camera.h>
#include <Game/World/Dimension/Dimension.h>
#include <iostream>

Chunk::Chunk(Dimension* dim, ChunkPosition _position)
	: render(this)
{
	position = _position;

	dimension = dim;
}

Chunk::~Chunk()
{
	for (int i = 0; i < CHUNK_SIZE; i++) {
		blocks[i]->Destroy();
	}
}

Chunk* Chunk::LoadChunk(Dimension* _dimension, ChunkPosition _position)
{
	Chunk* chunk = new Chunk(_dimension, _position);
	if (true) { // TODO: check if the chunk data has been saved.
		chunk->GenerateFreshChunkTerrain();
	}

	return chunk;
}

void Chunk::UnloadChunk()
{
	delete this;
}

void Chunk::GenerateFreshChunkTerrain()
{
	blocks.TestSetAllBlocks();
}

void Chunk::DrawChunk()
{
	render.Draw();
}

void Chunk::Init()
{
	RegenerateChunkMeshData();
}

Block* Chunk::GetBlockAtLocalPosition(BlockPosition bpos)
{
#ifdef DEVELOPMENT
	if (bpos.x >= CHUNK_WIDTH || bpos.y >= CHUNK_WIDTH || bpos.z >= CHUNK_WIDTH) {
		return nullptr;
	}
#endif

	return blocks.GetBlock(bpos);
}

void Chunk::RegenerateChunkMeshData()
{
	render.AsyncConstructBlockMesh();
}

glm::vec3 Chunk::ShiftToRenderOrigin()
{
	const glm::dvec3 playerOffset = WORLD_ORIGIN - Player::GetPlayer()->GetPosition();
	glm::dvec3 chunkRenderPos{ 
		position.x * CHUNK_WIDTH + playerOffset.x,
		position.y * CHUNK_WIDTH + playerOffset.y,
		position.z * CHUNK_WIDTH + playerOffset.z
	};

	// Start with initial high precision then when chunk is shifted towards player, compress to floats for gpu.
	return glm::vec3(chunkRenderPos);
}

bool Chunk::IsChunkInCamera(glm::vec3& outRenderPos)
{
	// Any chunk within this distance will not be culled
	constexpr float CHUNK_CLOSE_CULL = 32.f;

	const Camera* cam = Camera::GetActiveCamera();

	const glm::vec3 renderPos = ShiftToRenderOrigin();

	const float sqrOriginDist = renderPos.x * renderPos.x + renderPos.y * renderPos.y + renderPos.z * renderPos.z;
	const float sqrNearDist = 0.1f * 0.1f;
	const float sqrFarDist = (float(chunkRenderDistance) * 16.f) * (float(chunkRenderDistance) * 16.f);

	if (sqrOriginDist < (CHUNK_CLOSE_CULL * CHUNK_CLOSE_CULL)) {
		outRenderPos = renderPos;
		return true;
	}

	//if (sqrOriginDist < sqrNearDist || sqrOriginDist > sqrFarDist) {
	//	return false;
	//}

	const glm::mat4 mvp = cam->GetProjectionMatrix() * cam->GetViewMatrix();
	const glm::vec4 clip = mvp * glm::vec4(renderPos, 1);

	const bool chunkPointOut = abs(renderPos.x) > clip.w && abs(renderPos.y) > clip.w;
	const bool chunkOppositePointOut = abs(renderPos.x + 16.f) > clip.w && abs(renderPos.y + 16.f) > clip.w;

	//if (chunkPointOut && chunkOppositePointOut) {
	//	return false;
	//}

	outRenderPos = renderPos;
	return true;
}
