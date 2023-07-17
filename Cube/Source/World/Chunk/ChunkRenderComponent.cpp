#include "ChunkRenderComponent.h"
#include "../../Graphics/OpenGL/Buffers/VertexBufferObject.h"
#include "../../Graphics/OpenGL/Buffers/IndexBufferObject.h"
#include "../../Graphics/OpenGL/Buffers/VertexArrayObject.h"
#include "Chunk.h"
#include "../Block/Block.h"
#include "../Render/ChunkRenderer.h"
#include <chrono>
#include "../../GameInstance.h"
#include "../../Graphics/Geometry/BlockGeometry.h"
#include "../World.h"
#include "../Block/BlockTypes/Air/AirBlock.h"

ChunkRenderComponent::ChunkRenderComponent(Chunk* chunkOwner)
	: chunk(chunkOwner), emptyMesh(true)
{
	mesh = new ChunkMesh();
	// It's possible OpenGL won't like these being created on the primary thread. Investigate if so.
	vbos = new PersistentMappedTripleBuffer<VertexBufferObject, BlockQuad>();
	ibos = new PersistentMappedTripleBuffer<IndexBufferObject, uint32>();
}

ChunkRenderComponent::~ChunkRenderComponent()
{
	delete mesh;
	// Potentially will need to delete buffers using render thread
	delete vbos;
	delete ibos;
}

void ChunkRenderComponent::RecreateMesh()
{
	mesh->Empty();
	const GlobalString airName = AirBlock::GetStaticName();

	for (int i = 0; i < CHUNK_SIZE; i++) {
		const BlockPosition blockPos = i;
		const Block* block = chunk->GetBlock(blockPos);

		if (block->GetName() == airName) continue;

		const WorldPosition worldPos = WorldPosition::FromChunkAndBlock(chunk->GetPosition(), blockPos);
		const glm::vec3 vertexOffset{ blockPos.X(), blockPos.Y(), blockPos.Z() };
		block->AddBlockMeshToChunkMesh(mesh, chunk, worldPos, vertexOffset);
	}
	emptyMesh = mesh->GetQuadCount() == 0;
	if (emptyMesh) {
		cubeLog("Empty chunk mesh for chunk " + String::From(chunk->GetPosition()));
	}
	chunk->SetShouldBeRemeshed(false);
}

void ChunkRenderComponent::MultithreadRecreateMeshes(const darray<Chunk*>& chunks, gk::ThreadPool* threadPool)
{
	gk_assertNotNull(threadPool);
	gk_assert(threadPool->AllThreadsReady());
#if true
	for (Chunk* chunk : chunks) {
		auto func = std::bind(&ChunkRenderComponent::RecreateMesh, chunk->GetRenderComponent());
		threadPool->AddFunctionToQueue(func);
		//component->RecreateMesh(chunkRenderer->GetChunkMesh(component->GetChunk()));
	}
	threadPool->ExecuteQueue();
#else
	for (Chunk* chunk : chunks) {
		auto func = std::bind(&ChunkRenderComponent::CalculateBuriedBitmask, chunk->GetRenderComponent());
		threadPool->AddFunctionToQueue(func);
	}
	threadPool->ExecuteQueue();
	for (Chunk* chunk : chunks) {
		auto func = std::bind(&ChunkRenderComponent::RecreateMeshUsingBuriedBitmaskAndAdjacentTest, chunk->GetRenderComponent());
		threadPool->AddFunctionToQueue(func);
	}
	threadPool->ExecuteQueue();
#endif
}

void ChunkRenderComponent::CalculateBuriedBitmask()
{
	buriedBitmask.Reset();
	const ChunkPosition chunkPos = chunk->GetPosition();
	const MappedAdjacentChunks adjacentChunks = MappedAdjacentChunks::Create(GetWorld(), chunkPos);

	for (int i = 0; i < CHUNK_SIZE; i++) {
		const BlockPosition blockPos = i;
		const WorldPosition worldPos = WorldPosition::FromChunkAndBlock(chunkPos, blockPos);
		const bool isBlockBuried = adjacentChunks.GetBlock(worldPos)->IsBuried(adjacentChunks, worldPos);
		buriedBitmask.SetBlockBuriedState(blockPos, isBlockBuried);
	}
}

void ChunkRenderComponent::RecreateMeshUsingBuriedBitmaskAndAdjacentTest()
{
	mesh->Empty();
	const ChunkPosition chunkPos = chunk->GetPosition();
	const MappedAdjacentAndBuriedChunks adjacentChunks = MappedAdjacentAndBuriedChunks::Create(GetWorld(), chunkPos);
	const GlobalString airName = AirBlock::GetStaticName();

	for (int i = 0; i < CHUNK_SIZE; i++) {
		const BlockPosition blockPos = i;
		const Block* block = chunk->GetBlock(blockPos);

		if (block->GetName() == airName || buriedBitmask.IsBlockBuried(blockPos)) continue;

		const WorldPosition worldPos = WorldPosition::FromChunkAndBlock(chunkPos, blockPos);
		const glm::vec3 vertexOffset{ blockPos.X(), blockPos.Y(), blockPos.Z() };
		block->AddBlockMeshToChunkMeshBitmaskTest(mesh, chunk, worldPos, vertexOffset, adjacentChunks);
	}
	emptyMesh = mesh->GetQuadCount() == 0;
	if (emptyMesh) {
		cubeLog("Empty chunk mesh for chunk " + String::From(chunk->GetPosition()));
	}
	chunk->SetShouldBeRemeshed(false);
}


