#include "ChunkRenderComponent.h"
#include "../../Graphics/OpenGL/Buffers/VertexBufferObject.h"
#include "../../Graphics/OpenGL/Buffers/IndexBufferObject.h"
#include "../../Graphics/OpenGL/Buffers/VertexArrayObject.h"
#include "Chunk.h"
#include "../Block/IBlock.h"
#include "../Render/ChunkRenderer.h"
#include <chrono>
#include "../../GameInstance.h"
#include "../../Graphics/Geometry/BlockGeometry.h"
#include "../World.h"

ChunkRenderComponent::ChunkRenderComponent(Chunk* chunkOwner)
	: chunk(chunkOwner), meshWasRecreated(false), meshRequiresLargerVbo(false), meshRequiresLargerIbo(false), isMeshEmpty(false)
{
	vbos = new PersistentMappedTripleBuffer<VertexBufferObject, BlockQuad>();
	ibos = new PersistentMappedTripleBuffer<IndexBufferObject, uint32>();
}

ChunkRenderComponent::~ChunkRenderComponent()
{
	if (vbos) delete vbos;
	if (ibos) delete ibos;
}

void ChunkRenderComponent::RecreateMesh(ChunkMesh* mesh)
{
	meshWasRecreated = true;
	mesh->Empty();
	const IBlock* air = BlockFactory::GetAirBlock();

	for (int i = 0; i < CHUNK_SIZE; i++) {
		const BlockPosition blockPos = i;
		const IBlock* block = chunk->GetBlock(blockPos);

		if (block == air) continue;

		const WorldPosition worldPos = WorldPosition::FromChunkAndBlock(chunk->GetPosition(), blockPos);
		const glm::vec3 vertexOffset{ blockPos.X(), blockPos.Y(), blockPos.Z() };
		block->AddBlockMeshToChunkMesh(mesh, chunk, worldPos, vertexOffset);
	}

	isMeshEmpty = mesh->GetQuadCount() == 0;
	if (mesh->GetQuadCount() > GetMaximumQuadsPerChunkMesh()) {
		cubeLog(
			string("[Error]: ChunkRenderComponent::RecreateMesh()... Number of generated quads exceeds maximum allowed per chunk. Generated: ") 
			+ string::FromInt(mesh->GetQuadCount()) 
			+ string(", Allowed: ") 
			+ string::FromInt(GetMaximumQuadsPerChunkMesh()));
	}

	//int bytesUsedByMesh = sizeof(BlockQuad) * size_t(mesh.GetQuads().Capacity());
	//std::cout << "Bytes used by mesh: " << bytesUsedByMesh << std::endl;

	TryCopyMeshQuadsToVbo();
	TryCopyMeshIndicesToIbo();
}

void ChunkRenderComponent::MultithreadRecreateMeshes(const ChunkRenderer* chunkRenderer, const darray<ChunkRenderComponent*>& components)
{
	cubeLog(string("Multithread - recreating ") + string::FromInt(components.Size()) + string(" chunk meshes"));
	gk::ThreadPool* threadPool = GetGameInstance()->GetThreadPool();
	for (ChunkRenderComponent* component : components) {
		auto func = std::bind(&ChunkRenderComponent::RecreateMesh, component, chunkRenderer->GetChunkMesh(component->GetChunk()));
		threadPool->AddFunctionToQueue(func);
	}

	threadPool->ExecuteQueue();
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
	const ChunkMesh* mesh = GetMesh();
	if (isMeshEmpty) return;

	VertexBufferObject* drawVbo = vbos->GetBoundBuffer();
	IndexBufferObject* drawIbo = ibos->GetBoundBuffer();

	/* It's possible that the mesh will not have been sent to the gpu yet, or it's using buffers that haven't been swapped yet.
	As a result, the valid buffers can be set for the next frame, and drawing this chunk during this frame can be skipped. */
	if (drawVbo && drawIbo) { 
		renderer->BindBlocksVertexBufferObject(drawVbo);
		renderer->SetShaderChunkOffset(renderer->GetOffsetForChunkDraw(chunk));
		renderer->Draw(drawVbo, drawIbo);
	}
	
	if (!meshWasRecreated) { // This takes a lot of cpu cycles. Investigate an alternative solution.
		return;
	}

	// Reserve extra capacity to be able to be able to easily handle changes to the mesh
	#define CAPACITY_INCREASE_FACTOR 1.5

	if (meshRequiresLargerVbo) {
		meshRequiresLargerVbo = false;
		vbos->Reserve((mesh->GetQuadCount() * CAPACITY_INCREASE_FACTOR));
		TryCopyMeshQuadsToVbo();
		checkm(!meshRequiresLargerVbo, "Mesh VBO should have enough capacity");
	}

	if (meshRequiresLargerIbo) {
		meshRequiresLargerIbo = false;
		ibos->Reserve((mesh->GetIndexCount() * CAPACITY_INCREASE_FACTOR));
		TryCopyMeshIndicesToIbo();
		checkm(!meshRequiresLargerIbo, "Mesh IBO should have enough capacity");
	}
	
	// Updated chunk information will be drawn next frame.
	vbos->SwapNextBuffer();
	ibos->SwapNextBuffer();
	meshWasRecreated = false;
}

uint32 ChunkRenderComponent::GetMaximumQuadsPerChunkMesh()
{
	// Amount of blocks per chunk * faces per block (this may need to be changed in the future for blocks with complex meshes)
	return CHUNK_SIZE * 6;
}

uint32 ChunkRenderComponent::GetMaximumIndicesPerChunkMesh()
{
	// Amount of blocks per chunk * faces per block * indices per face (this may need to be changed in the future for blocks with complex meshes)
	return CHUNK_SIZE * 6 * 6;
}

DrawElementsIndirectCommand ChunkRenderComponent::GenerateDrawElementsIndirectCommand(uint32 baseVertex, uint32 gl_InstanceId) const
{
	DrawElementsIndirectCommand command;
	command.count = GetMesh()->GetIndexCount();
	command.instanceCount = 1;
	command.firstIndex = 0;
	command.baseVertex = baseVertex;
	command.baseInstance = gl_InstanceId;

	std::cout << "draw count: " << command.count << std::endl;
	return command;
}

ChunkMesh* ChunkRenderComponent::GetMesh() const
{
	return GetWorld()->GetChunkRenderer()->GetChunkMesh(chunk);
}

//void ChunkRenderComponent::CopyMeshQuadsToVboOffset(PersistentMappedTripleVbo<BlockQuad>::MappedVbo& mappedVbo, uint32 quadMemoryOffset) const
//{
//	const ChunkMesh* mesh = GetMesh();
//	const uint32 quadCount = mesh->GetQuadCount();
//	mesh->CopyQuadsToBuffer(mappedVbo.data + quadMemoryOffset);
//	//mappedVbo.size = quadCount;
//}
//
//void ChunkRenderComponent::CopyMeshIndicesToIboOffset(PersistentMappedTripleIbo::MappedIbo& mappedIbo, uint32 integerMemoryOffset) const
//{
//	const ChunkMesh* mesh = GetMesh();
//	check(mappedIbo.data);
//	const uint32 indexCount = mesh->GetIndexCount();
//	mesh->CopyIndicesToBuffer(mappedIbo.data + integerMemoryOffset, 0);
//	mappedIbo.ibo->SetIndexCount(indexCount);
//	//mappedIbo.size = indexCount;
//}
//
//void ChunkRenderComponent::CopyDrawCommandToIndirectOffset(PersistentMappedTripleIndirect::MappedIndirect& mappedIndirect, uint32 commandMemoryOffset, DrawElementsIndirectCommand command) const
//{
//	check(mappedIndirect.data);
//	memcpy(mappedIndirect.data + commandMemoryOffset, &command, sizeof(DrawElementsIndirectCommand));
//}

void ChunkRenderComponent::TryCopyMeshQuadsToVbo()
{
	const ChunkMesh* mesh = GetMesh();
	const uint32 quadCount = mesh->GetQuadCount();
	const uint32 vboCapacity = vbos->GetCapacity();

	if (vboCapacity == 0 && quadCount == 0) {
		return;
	}

	if (vboCapacity < quadCount) {
		meshRequiresLargerVbo = true;
		return;
	}

	auto& mappedVbo = vbos->GetModifyMapped();
	mesh->CopyQuadsToBuffer(mappedVbo.data);
	//mappedVbo.size = quadCount;
}

void ChunkRenderComponent::TryCopyMeshIndicesToIbo()
{
	const ChunkMesh* mesh = GetMesh();
	const uint32 indexCount = mesh->GetIndexCount();
	const uint32 iboCapacity = ibos->GetCapacity();

	if (iboCapacity == 0 && indexCount == 0) {
		return;
	}

	if (iboCapacity < indexCount) {
		meshRequiresLargerIbo = true;
		return;
	}

	auto& mappedIbo = ibos->GetModifyMapped();
	mesh->CopyIndicesToBuffer(mappedIbo.data, 0);
	mappedIbo.buffer->SetIndexCount(indexCount);
	//mappedIbo.size = indexCount;
}