#include "Chunk.h"
#include <Game/World/World.h>
#include <Game/World/Block/Block.h>
#include <Engine/Render/Renderer.h>
#include <Engine/Render/Shader/Shader.h>
#include <Util/Benchmark/Benchmark.h>
#include <Game/World/Block/Factory/BlockFactory.h>
#include <Game/Player/Player.h>
#include <Engine/Render/Camera/Camera.h>
#include <Game/World/Dimension/Dimension.h>
#include <iostream>

Chunk::Chunk(Dimension* dim, ChunkPosition _position)
{
	indexBuffer = nullptr;
	vertexBuffer = nullptr;
	vertexArray = nullptr;

	canRender = false;

	position = _position;

	quadsToDraw = 0;

	dimension = dim;

	//Print("constructing chunk at position " + ToString({ position.x, position.y, position.z }));
}

Chunk::~Chunk()
{
	//Print("chunk destructor called");
	delete vertexArray;
	delete vertexBuffer;
	delete indexBuffer;
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
	int blockIndex = 0;
	for (uint8 y = 0; y < CHUNK_WIDTH; y++) {
		for (uint8 z = 0; z < CHUNK_WIDTH; z++) {
			for (uint8 x = 0; x < CHUNK_WIDTH; x++) {
				const BlockPosition bpos = { x, y, z };
				const WorldPosition wpos = WorldPosition::ToWorldPosition(position, bpos);
				blocks[blockIndex] = dimension->GetBlockForWorldPosition(wpos);
				blockIndex++;
			}
		}
	}
}

void Chunk::DrawChunk()
{
	if (canRender == false) return;

#ifdef DEVELOPMENT
	if (!vertexArray) return;
	if (!vertexBuffer) return;
	if (!indexBuffer) return;
#endif

	glm::vec3 shiftChunkPos;
	const bool chunkVisible = IsChunkInCamera(shiftChunkPos);
	if (!chunkVisible) return;

	vertexArray->Bind();
	vertexBuffer->Bind();
	indexBuffer->Bind();

	//Shader::GetBoundShader()->SetUniform3float("u_ChunkPosition", glm::vec3(position.x * CHUNK_WIDTH, position.y * CHUNK_WIDTH, position.z * CHUNK_WIDTH));
	//Print(ToString(ShiftToRenderOrigin()) + " current chu");
	Shader::GetBoundShader()->SetUniform3float("u_ChunkPosition", shiftChunkPos);
	Renderer::DrawQuads(quadsToDraw);
}

void Chunk::Init()
{
	RegenerateChunkMeshData();
}

Block* Chunk::GetBlockAtLocalPosition(BlockPosition bpos)
{
	if (bpos.x >= CHUNK_WIDTH || bpos.y >= CHUNK_WIDTH || bpos.z >= CHUNK_WIDTH) {
		return nullptr;
	}

	uint32 index = 0;
	index += bpos.x;
	index += bpos.z * CHUNK_WIDTH;
	index += bpos.y * CHUNK_WIDTH * CHUNK_WIDTH;

	if (index >= CHUNK_SIZE) {
		return nullptr;
	}

	return blocks[index];
}

BlockPosition Chunk::BlockIndexToRelativeLocation(uint32 index)
{
#ifdef DEVELOPMENT
	if (index >= CHUNK_SIZE) {
		Print("Chunk indexing for block relative location of " + ToString(index) + " exceeds the maximum index of " + ToString(CHUNK_SIZE));
		return { 0, 0, 0 };
	}
#endif

	BlockPosition pos;
	pos.x = index % CHUNK_WIDTH;
	pos.y = index / (CHUNK_WIDTH * CHUNK_WIDTH); // integer flooring
	pos.z = (index % (CHUNK_WIDTH * CHUNK_WIDTH)) / CHUNK_WIDTH; // compress index into range 0-255, then integer division
	return pos;
}

void Chunk::RegenerateChunkMeshData()
{
	if (indexBuffer) {
		delete indexBuffer;
	}
	if (vertexBuffer) {
		delete vertexBuffer;
	}
	if (vertexArray) {
		delete vertexArray;
	}

	std::vector<BlockQuad> quads;

	BlockQuad quadBuffer[6];
	uint32 generatedQuads;

	for (int i = 0; i < CHUNK_SIZE; i++) {
		Block* block = blocks[i];

		BlockPosition relativeLocation = BlockIndexToRelativeLocation(i);
		WorldPosition worldPos = {
			position.x * 16 + relativeLocation.x,
			position.y * 16 + relativeLocation.y,
			position.z * 16 + relativeLocation.z
		};

		if (block->GenerateBlockQuads(this, worldPos, { relativeLocation, 0, 0 }, quadBuffer, generatedQuads)) {
			quads.insert(quads.end(), quadBuffer, quadBuffer + generatedQuads);
		}
	}

	if (quads.size() == 0) {
		canRender = false;
		return;
	}

	vertexArray = new VertexArray();
	vertexBuffer = new VertexBuffer(&quads[0], sizeof(BlockQuad) * quads.size());  

	VertexBufferLayout layout = VertexBufferLayout::MakeVertexBufferLayoutForBlock();
	vertexArray->AddBuffer(vertexBuffer, layout);

	indexBuffer = IndexBuffer::MakeQuadsIndexBuffer(quads.size());

	quadsToDraw = quads.size();
	canRender = true;
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
