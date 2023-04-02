#pragma once

#include "ChunkSSBO.h"
#include "../../Graphics/Geometry/ChunkMesh.h"
#include "../../Graphics/OpenGL/Buffers/PersistentMappedTripleVbo.h"
#include "../../Graphics/OpenGL/Buffers/PersistentMappedTripleIbo.h"
#include "../../Graphics/OpenGL/Buffers/PersistentMappedTripleIndirect.h"
#include "../../Graphics/OpenGL/OpenGLStructures.h"

class VertexBufferObject;
class IndexBufferObject;
class ShaderBufferObject;
class VertexArrayObject;
class Shader;
class Chunk;
class ChunkRenderer;

class ChunkRenderComponent
{
public:

	ChunkRenderComponent(Chunk* chunkOwner);

	~ChunkRenderComponent();

	/* Recreates the mesh on the CPU side. Logic must multithread safe, performing no write operations to the chunk, blocks, or OpenGL. */
	void RecreateMesh();

	/* See RecreateMesh() */
	static void MultithreadRecreateMeshes(const darray<ChunkRenderComponent*>& components);
	
	void Draw(ChunkRenderer* renderer);

	/* Maximum number of BlockQuad's per chunk mesh to use for multidraw vbo capacity. */
	static uint32 GetMaximumQuadsPerChunkMesh();

	/* Maximum number of indices per chunk mesh to use for multidraw ibo capacity. */
	static uint32 GetMaximumIndicesPerChunkMesh();

	DrawElementsIndirectCommand GenerateDrawElementsIndirectCommand(uint32 baseVertex, uint32 gl_InstanceId) const;

	const ChunkMesh& GetMesh() const { return mesh; }

	void CopyMeshQuadsToVboOffset(PersistentMappedTripleVbo<BlockQuad>::MappedVbo& mappedVbo, uint32 quadMemoryOffset) const;

	void CopyMeshIndicesToIboOffset(PersistentMappedTripleIbo::MappedIbo& mappedIbo, uint32 integerMemoryOffset) const;

	void CopyDrawCommandToIndirectOffset(PersistentMappedTripleIndirect::MappedIndirect& mappedIndirect, uint32 commandMemoryOffset, DrawElementsIndirectCommand command) const;

	bool IsMeshEmpty() const { return isMeshEmpty; }

private:

	/* Attempts to copy the mesh's BlockQuad data to the modifiable vertex buffer object. 
	If the vbo does not have enough space, meshRequiresLargeVbo will be set to true. */
	void TryCopyMeshQuadsToVbo();

	/* Attempts to copy the mesh's index data to the modifiable index buffer object.
	If the ibo does not have enough space, meshRequiresLargeIbo will be set to true. */
	void TryCopyMeshIndicesToIbo();

private:

	/* The chunk that owns this render component, and who's mesh will be created and drawn from. */
	Chunk* chunk;

	/* The mesh that is stored on the cpu, to later be sent to the gpu through the persistent mapped buffers. */
	ChunkMesh mesh;

	/* Persistently mapped triple buffered VBOs for faster gpu data syncronization. 
	This will have a capacity (like a dynamic array) that should be able to hold more than or equal to the amount of quads required by the mesh.
	This means upon remeshing, the existing VBOs will likely be able to be used. If not, the VBOs will need to be reallocated. */
	PersistentMappedTripleVbo<BlockQuad>* vbos;

	/* Persistently mapped triple buffered IBOs for faster gpu data syncronization.
	This will have a capacity (like a dynamic array) that should be able to hold more than or equal to the amount of indices required by the mesh.
	This means upon remeshing, the existing IBOs will likely be able to be used. If not, the IBOs will need to be reallocated. */
	PersistentMappedTripleIbo* ibos;

	/**/
	bool isMeshEmpty;

	/* Tracks if the mesh for this chunk was recreated, if so, meaning a buffer swap / potential buffer reallocation will be required. Gets reset to false after every draw. */
	bool meshWasRecreated;

	/* Tracks if the recreated mesh requires a vbo capacity greater than the current vbo(s) can contain. Can only be true if meshWasRecreated is also true. */
	bool meshRequiresLargerVbo;

	/* Tracks if the recreated mesh requires an ibo capacity greater than the current ibo(s) can contain. Can only be true if meshWasRecreated is also true. */
	bool meshRequiresLargerIbo;
};