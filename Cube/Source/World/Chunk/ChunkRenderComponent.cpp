#include "ChunkRenderComponent.h"
#include "../../Graphics/OpenGL/Buffers/VertexBufferObject.h"
#include "../../Graphics/OpenGL/Buffers/IndexBufferObject.h"
#include "../../Graphics/OpenGL/Buffers/VertexArrayObject.h"
#include "Chunk.h"
#include "../Block/IBlock.h"
#include "../Render/ChunkRenderer.h"
#include <chrono>

ChunkRenderComponent::ChunkRenderComponent(Chunk* chunkOwner)
	: chunk(chunkOwner), meshWasRecreated(false)
{
	vbos = new PersistentMappedTripleVbo<BlockQuad>();
	ibos = new PersistentMappedTripleIbo();
}

ChunkRenderComponent::~ChunkRenderComponent()
{
	if (vbos) delete vbos;
	if (ibos) delete ibos;
}

void ChunkRenderComponent::RecreateMesh()
{
	meshWasRecreated = true;
	mesh.Empty();
	const IBlock* air = BlockFactory::GetAirBlock();

	for (int i = 0; i < CHUNK_SIZE; i++) {
		const BlockPosition blockPos = i;
		const IBlock* block = chunk->GetBlock(blockPos);

		if (block == air) continue;

		const WorldPosition worldPos = WorldPosition::FromChunkAndBlock(chunk->GetPosition(), blockPos);
		const glm::vec3 vertexOffset{ blockPos.X(), blockPos.Y(), blockPos.Z() };
		block->AddBlockMeshToChunkMesh(mesh, chunk, worldPos, vertexOffset);
	}

	/* Reserve more vram capacity than required to reduce the necessity for recreating the buffers. */
	const uint32 quadCount = mesh.GetQuadCount();
	const uint32 indexCount = mesh.GetIndexCount();
	if (vbos->GetCapacity() < quadCount) {
		vbos->Reserve(quadCount * 2); 
	}
	if (ibos->GetCapacity() < indexCount) {
		ibos->Reserve(indexCount * 2);
	}
	PersistentMappedTripleVbo<BlockQuad>::MappedVbo mappedVbo = vbos->GetModifyMappedVbo();
	PersistentMappedTripleIbo::MappedIbo mappedIbo = ibos->GetModifyMappedIbo();
	mesh.CopyQuadsToBuffer(mappedVbo.data);
	mesh.CopyIndicesToBuffer(mappedIbo.data);
	mappedIbo.ibo->SetIndexCount(mesh.GetIndexCount());
}

//void ChunkRenderComponent::MeshToOpenGLObjects()
//{
//	if (vbo) delete vbo;
//	if (ibo) delete ibo;
//
//	vbo = mesh.MakeVertexBufferObject();
//	ibo = mesh.MakeIndexBufferObject();
//	mesh.Empty();
//	meshWasRecreated = false;
//}

void ChunkRenderComponent::Draw(ChunkRenderer* renderer)
{
	VertexBufferObject* drawVbo = vbos->GetBoundMappedVbo().vbo;
	IndexBufferObject* drawIbo = ibos->GetBoundMappedIbo().ibo;
	renderer->BindBlocksVertexBufferObject(drawVbo);
	renderer->Draw(drawVbo, drawIbo);

	if (!meshWasRecreated) { 
		return;
	}
	// Updated chunk information will be drawn next frame.
	vbos->SwapNextBuffer();
	ibos->SwapNextBuffer();
	meshWasRecreated = false;
}
