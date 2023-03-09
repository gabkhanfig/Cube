#include "ChunkRenderComponent.h"
#include "../../Graphics/OpenGL/Buffers/VertexBufferObject.h"
#include "../../Graphics/OpenGL/Buffers/IndexBufferObject.h"
#include "../../Graphics/OpenGL/Buffers/VertexArrayObject.h"
#include "Chunk.h"
#include "../Block/IBlock.h"
#include "../Render/ChunkRenderer.h"
#include <chrono>

ChunkRenderComponent::ChunkRenderComponent(Chunk* _owner)
	: owner(_owner), vbo(nullptr), ibo(nullptr), meshWasRecreated(false)
{}

ChunkRenderComponent::~ChunkRenderComponent()
{
	if (vbo) delete vbo;
	if (ibo) delete ibo;
}

void ChunkRenderComponent::RecreateMesh()
{
	mesh.Empty();
	const IBlock* air = BlockFactory::GetAirBlock();

	for (int i = 0; i < CHUNK_SIZE; i++) {
		const BlockPosition blockPos = i;
		const IBlock* block = owner->GetBlock(blockPos);

		if (block == air) continue;

		const WorldPosition worldPos = WorldPosition::FromChunkAndBlock(owner->GetPosition(), blockPos);
		block->AddBlockMeshToChunkMesh(mesh, owner, worldPos);
	}
}

void ChunkRenderComponent::MeshToOpenGLObjects()
{
	if (vbo) delete vbo;
	if (ibo) delete ibo;

	auto start1 = std::chrono::high_resolution_clock::now();
	vbo = mesh.MakeVertexBufferObject();
	auto stop1 = std::chrono::high_resolution_clock::now();
	auto start2 = std::chrono::high_resolution_clock::now();
	ibo = mesh.MakeIndexBufferObject();
	auto stop2 = std::chrono::high_resolution_clock::now();
	std::cout << "Time to create VBO:  " << std::chrono::duration_cast<std::chrono::microseconds>(stop1 - start1).count() << "us\n";
	std::cout << "Time to create IBO: " << std::chrono::duration_cast<std::chrono::microseconds>(stop2 - start2).count() << "us\n";
	mesh.Empty();
	meshWasRecreated = false;
}

void ChunkRenderComponent::CopyToSameVBO()
{
	void* mapVBO = vbo->GetMapBuffer();
	memcpy(mapVBO, mesh.GetQuads().Data(), mesh.GetQuads().Size() * sizeof(BlockQuad));
	VertexBufferObject::UnmapBuffer();
	//uint32* mapIBO = ibo->GetMapBuffer();
	//memcpy(mapIBO, mesh.GetQuads().)
}

void ChunkRenderComponent::Draw(ChunkRenderer* renderer)
{
	if (meshWasRecreated) {
		MeshToOpenGLObjects();
	}

	renderer->BindBlocksVertexBufferObject(vbo);
	renderer->Draw(vbo, ibo);
}
