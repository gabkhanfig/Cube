#include "ChunkRenderComponent.h"
//#include "../../Graphics/OpenGL/Buffers/VertexBufferObject.h"
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
#include "../../Graphics/OpenGL/Buffers/MappedTripleIbo.h"
//#include "../../Graphics/OpenGL/Buffers/MappedTripleVbo.h"

#include "../../Player/Player.h"

ChunkRenderComponent::ChunkRenderComponent(Chunk* chunkOwner)
	: chunk(chunkOwner), emptyMesh(true), vbos(nullptr), ibos(nullptr)
{
	mesh = new ChunkMesh();
	// It's possible OpenGL won't like these being created on the primary thread. Investigate if so.
	//vbos = new PersistentMappedTripleBuffer<VertexBufferObject, BlockQuad>();5
	//vbos = new MappedTripleVbo<BlockQuad>();
	//ibos = new PersistentMappedTripleBuffer<IndexBufferObject, uint32>();
	//ibos = new MappedTripleIbo();
}

ChunkRenderComponent::~ChunkRenderComponent()
{
	delete mesh;
	// Potentially will need to delete buffers using render thread
	gk_assertm(false, "currently not deallocating mapped buffers");
	//delete vbos;
	//delete ibos;
}

void ChunkRenderComponent::RecreateMesh()
{
	mesh->Empty();
	const GlobalString airName = "air";

	for (int i = 0; i < CHUNK_SIZE; i++) {
		const BlockPosition blockPos = i;
		const Block* block = chunk->GetBlock(blockPos);

		if (block->GetName() == airName) continue;

		const WorldPosition worldPos = WorldPosition(chunk->GetPosition(), blockPos);
		const glm::vec3 vertexOffset{ blockPos.X(), blockPos.Y(), blockPos.Z() };
		//block->AddBlockMeshToChunkMesh(mesh, chunk, worldPos, vertexOffset);
	}
	emptyMesh = mesh->GetQuadCount() == 0;
	//if (emptyMesh) {
	//	cubeLog("Empty chunk mesh for chunk " + String::From(chunk->GetPosition()));
	//}
	chunk->SetShouldBeRemeshed(false);
}

void ChunkRenderComponent::MultithreadRecreateMeshes(const darray<Chunk*>& chunks, gk::ThreadPool* threadPool)
{
	cubeLog("Remeshing " + String::FromUint(chunks.Size()) + " chunks\n" + String::From(chunks.Size() * CHUNK_SIZE) + " blocks");
	gk_assertNotNull(threadPool);
	gk_assert(threadPool->AllThreadsReady());
#if false
	for (Chunk* chunk : chunks) {
		auto func = std::bind(&ChunkRenderComponent::RecreateMesh, chunk->GetRenderComponent());
		threadPool->AddFunctionToQueue(func);
		//component->RecreateMesh(chunkRenderer->GetChunkMesh(component->GetChunk()));
	}
	threadPool->ExecuteQueue();
#else
	//for (Chunk* chunk : chunks) {
	//	auto func = std::bind(&ChunkRenderComponent::CalculateSolidBitmask, chunk->GetRenderComponent());
	//	threadPool->AddFunctionToQueue(func);
	//}
	//threadPool->ExecuteQueue();

	for (Chunk* chunk : chunks) {
		auto func = std::bind(&ChunkRenderComponent::CalculateBuriedBitmask, chunk->GetRenderComponent());
		threadPool->AddFunctionToQueue(func);
	}
	threadPool->ExecuteQueue();

	for (Chunk* chunk : chunks) {
		auto func = std::bind(&ChunkRenderComponent::RecreateMeshUsingBuriedBitmaskAndAdjacentTest, chunk->GetRenderComponent());
		//func();
		threadPool->AddFunctionToQueue(func);
	}
	threadPool->ExecuteQueue();
	
#endif
}

void ChunkRenderComponent::MultithreadMemcpyDataAndCreateDrawCalls(const darray<Chunk*>& chunks, gk::ThreadPool* threadPool, darray<ChunkDrawCall>* drawCallsOut)
{
	for (Chunk* chunk : chunks) {
		auto func = std::bind(&ChunkRenderComponent::MemcpyMeshDataAndSwapBuffer, chunk->GetRenderComponent());
		threadPool->AddFunctionToQueue(func);
	}
	threadPool->ExecuteQueue();

	for (uint32 i = 0; i < chunks.Size(); i++) {
		drawCallsOut->Add(ChunkDrawCall());
	}

	uint32 chunkIndex = 0;
	for (const Chunk* chunk : chunks) {
		ChunkDrawCall* drawCall = &drawCallsOut->At(chunkIndex);
		auto func = std::bind(&ChunkRenderComponent::FillChunkDrawCallData, chunk->GetRenderComponent(), drawCall);
		threadPool->AddFunctionToQueue(func);
		chunkIndex++;
	}
	threadPool->ExecuteQueue();
}

void ChunkRenderComponent::MemcpyMeshDataAndSwapBuffer()
{
	gk_assertNotNull(vbos);
	gk_assertNotNull(ibos);
	gk_assertm(vbos->GetCapacity() >= mesh->GetQuadCount(), "vbos does not have enough capacity for mesh");
	gk_assertm(ibos->GetCapacity() >= mesh->GetIndexCount(), "ibos does not have enough capacity for mesh indices");

	auto& mappedVbo = vbos->GetModifyMapped();
	memcpy(mappedVbo.data, mesh->GetQuadsData(), mesh->GetQuadCount() * sizeof(BlockQuad));
	auto& mappedIbo = ibos->GetModifyMapped();
	memcpy(mappedIbo.data, mesh->GetIndices().Data(), mesh->GetIndexCount() * sizeof(uint32));
	mappedIbo.buffer->SetIndexCount(mesh->GetIndexCount());

	vbos->SwapNextBuffer();
	ibos->SwapNextBuffer();
}

void ChunkRenderComponent::FillChunkDrawCallData(ChunkDrawCall* drawCallOut) const
{
	gk_assertNotNull(drawCallOut);
	drawCallOut->chunk = chunk;
	drawCallOut->vbo = vbos->GetBoundBuffer();
	drawCallOut->ibo = ibos->GetBoundBuffer();
	drawCallOut->indicesToDraw = mesh->GetIndexCount();
	drawCallOut->chunkPositionOffset = ChunkRenderer::GetChunkShaderPositionOffset(GetWorld()->GetPlayer()->GetLocation(), chunk);
}

void ChunkRenderComponent::CalculateBuriedBitmask()
{
	buriedBitmask.Reset();
	const ChunkPosition chunkPos = chunk->GetPosition();
	const MappedAdjacentChunks adjacentChunks = MappedAdjacentChunks::Create(GetWorld(), chunkPos);

	for (int i = 0; i < CHUNK_SIZE; i++) {
		const BlockPosition blockPos = i;
		const WorldPosition worldPos = WorldPosition(chunkPos, blockPos);
		const Block* block = chunk->GetBlock(blockPos);
		const bool isBlockBuried = block->IsBuried(chunk, adjacentChunks, worldPos, blockPos);
		buriedBitmask.SetFlag(blockPos, isBlockBuried);
	}
}

void ChunkRenderComponent::RecreateMeshUsingBuriedBitmaskAndAdjacentTest()
{
	mesh->Empty();

	if (buriedBitmask.AreAllBlocksSet()) { // All blocks are buried
		emptyMesh = true;
		chunk->SetShouldBeRemeshed(false);
		return;
	}

	const ChunkPosition chunkPos = chunk->GetPosition();
	const MappedAdjacentAndBuriedChunks adjacentChunks = MappedAdjacentAndBuriedChunks::Create(GetWorld(), chunkPos);
	const GlobalString airName = "air";

	for (int i = 0; i < CHUNK_SIZE; i++) {
		const BlockPosition blockPos = i;
		const Block* block = chunk->GetBlock(blockPos);

		if (block->GetName() == airName || buriedBitmask.GetFlag(blockPos)) continue;

		const WorldPosition worldPos = WorldPosition(chunkPos, blockPos);
		const glm::vec3 vertexOffset{ blockPos.X(), blockPos.Y(), blockPos.Z() };
		block->ConstructMesh(mesh, chunk, worldPos, vertexOffset, adjacentChunks);
		//block->AddBlockMeshToChunkMeshBitmaskTest(mesh, chunk, worldPos, vertexOffset, adjacentChunks);
	}
	emptyMesh = mesh->GetQuadCount() == 0;
	//if (emptyMesh) {
	//	cubeLog("Empty chunk mesh for chunk " + String::From(chunk->GetPosition()));
	//}
	chunk->SetShouldBeRemeshed(false);
}

bool ChunkRenderComponent::AreGLBuffersInitialized() const
{
	return vbos != nullptr && ibos != nullptr;
}

void ChunkRenderComponent::CreateGLBuffers()
{
	if (vbos != nullptr) {
		delete vbos;
	}
	if (ibos != nullptr) {
		delete ibos;
	}

	vbos = new MappedTripleVbo<BlockQuad>(); 
	ibos = new MappedTripleIbo();
}


