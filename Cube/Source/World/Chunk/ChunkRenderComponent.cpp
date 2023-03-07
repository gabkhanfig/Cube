#include "ChunkRenderComponent.h"
#include "../../Graphics/OpenGL/Buffers/VertexBufferObject.h"
#include "../../Graphics/OpenGL/Buffers/IndexBufferObject.h"
#include "../../Graphics/OpenGL/Buffers/VertexArrayObject.h"
#include "Chunk.h"
#include "../Block/IBlock.h"
#include "../Render/ChunkRenderer.h"

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

	vbo = mesh.MakeVertexBufferObject();
	ibo = mesh.MakeIndexBufferObject();
	mesh.Empty();
	meshWasRecreated = false;
}

void ChunkRenderComponent::Draw(ChunkRenderer* renderer)
{
	if (meshWasRecreated) {
		MeshToOpenGLObjects();
	}

	renderer->BindBlocksVertexBufferObject(vbo);
	renderer->Draw(vbo, ibo);
}
