#include "ChunkRenderComponent.h"
#include "../../Graphics/OpenGL/Buffers/VertexBufferObject.h"
#include "../../Graphics/OpenGL/Buffers/IndexBufferObject.h"
#include "../../Graphics/OpenGL/Buffers/VertexArrayObject.h"
#include "Chunk.h"
#include "../Block/IBlock.h"
#include "../Render/ChunkRenderer.h"
#include <chrono>

ChunkRenderComponent::ChunkRenderComponent(Chunk* chunkOwner)
	: chunk(chunkOwner), meshWasRecreated(false), meshRequiresLargerVbo(false), meshRequiresLargerIbo(false)
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

	TryCopyMeshQuadsToVbo();
	TryCopyMeshIndicesToIbo();
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

	/* It's possible that the mesh will not have been sent to the gpu yet, or it's using buffers that haven't been swapped yet.
	As a result, the valid buffers can be set for the next frame, and drawing this chunk during this frame can be skipped. */
	if (drawVbo && drawIbo) { 
		renderer->BindBlocksVertexBufferObject(drawVbo);
		renderer->SetShaderChunkOffset(renderer->GetOffsetForChunkDraw(chunk));
		renderer->Draw(drawVbo, drawIbo);
	}
	
	if (!meshWasRecreated) {
		return;
	}

	// Reserve extra capacity to be able to be able to easily handle changes to the mesh
	#define CAPACITY_INCREASE_FACTOR 1.5

	if (meshRequiresLargerVbo) {
		meshRequiresLargerVbo = false;
		vbos->Reserve((mesh.GetQuadCount() * CAPACITY_INCREASE_FACTOR));
		TryCopyMeshQuadsToVbo();
		checkm(!meshRequiresLargerVbo, "Mesh VBO should have enough capacity");
	}

	if (meshRequiresLargerIbo) {
		meshRequiresLargerIbo = false;
		ibos->Reserve((mesh.GetIndexCount() * CAPACITY_INCREASE_FACTOR));
		TryCopyMeshIndicesToIbo();
		checkm(!meshRequiresLargerIbo, "Mesh IBO should have enough capacity");
	}
	
	// Updated chunk information will be drawn next frame.
	vbos->SwapNextBuffer();
	ibos->SwapNextBuffer();
	meshWasRecreated = false;
}

void ChunkRenderComponent::TryCopyMeshQuadsToVbo()
{
	const uint32 quadCount = mesh.GetQuadCount();
	if (vbos->GetCapacity() < quadCount) {
		meshRequiresLargerVbo = true;
		return;
	}

	PersistentMappedTripleVbo<BlockQuad>::MappedVbo& mappedVbo = vbos->GetModifyMappedVbo();
	mesh.CopyQuadsToBuffer(mappedVbo.data);
	mappedVbo.size = quadCount;
}

void ChunkRenderComponent::TryCopyMeshIndicesToIbo()
{
	const uint32 indexCount = mesh.GetIndexCount();
	if (ibos->GetCapacity() < indexCount) {
		meshRequiresLargerIbo = true;
		return;
	}

	PersistentMappedTripleIbo::MappedIbo& mappedIbo = ibos->GetModifyMappedIbo();
	mesh.CopyIndicesToBuffer(mappedIbo.data);
	mappedIbo.ibo->SetIndexCount(mesh.GetIndexCount());
	mappedIbo.size = indexCount;
}