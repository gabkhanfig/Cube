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
	: chunk(chunkOwner)
{
	mesh = new ChunkMesh();
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
}

void ChunkRenderComponent::MultithreadRecreateMeshes(const ChunkRenderer* chunkRenderer, const darray<ChunkRenderComponent*>& components)
{
	cubeLog(string("Multithread - recreating ") + string::FromInt(components.Size()) + string(" chunk meshes"));
	gk::ThreadPool* threadPool = GetGameInstance()->GetThreadPool();
	for (ChunkRenderComponent* component : components) {
		auto func = std::bind(&ChunkRenderComponent::RecreateMesh, component);
		threadPool->AddFunctionToQueue(func);
		//component->RecreateMesh(chunkRenderer->GetChunkMesh(component->GetChunk()));
	}

	threadPool->ExecuteQueue();
}