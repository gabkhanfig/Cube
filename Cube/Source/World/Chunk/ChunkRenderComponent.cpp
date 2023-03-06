#include "ChunkRenderComponent.h"
#include "../../Graphics/OpenGL/Buffers/VertexBufferObject.h"
#include "../../Graphics/OpenGL/Buffers/IndexBufferObject.h"
#include "../../Graphics/OpenGL/Buffers/VertexArrayObject.h"
#include "../../Graphics/OpenGL/Render/Renderer.h"
#include "Chunk.h"
#include "../Block/IBlock.h"

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
		const BlockPosition blockPos = BlockPosition::FromBlockIndex(i);
		const WorldPosition worldPos = WorldPosition::FromChunkAndBlock(owner->GetPosition(), blockPos);

		const IBlock* block = owner->GetBlock(blockPos);
		if (block == air)
			continue;

		block->AddBlockMeshToChunkMesh(mesh, owner, worldPos);
		//BlockMesh blockMesh = block->AddBlockMeshToChunk(owner, worldPos);
		//blockMesh.Shift(glm::vec3(blockPos.x, blockPos.y, blockPos.z));
		//mesh.AddBlockMesh(blockMesh);
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

void ChunkRenderComponent::Draw(Shader* chunkShader, VertexArrayObject* chunkVAO)
{
	if (meshWasRecreated) {
		MeshToOpenGLObjects();
	}

	chunkVAO->BindVertexBufferObject(vbo, sizeof(BlockVertex));
	Renderer::DrawVboTriangles(vbo, ibo);
}
