#pragma once

#include "ChunkSSBO.h"
#include "../../Graphics/Geometry/ChunkMesh.h"
#include "../../Graphics/OpenGL/Buffers/PersistentMappedTripleVbo.h"
#include "../../Graphics/OpenGL/Buffers/PersistentMappedTripleIbo.h"

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

	void Draw(ChunkRenderer* renderer);

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

	/* Tracks if the mesh for this chunk was recreated, if so, meaning a buffer swap / potential buffer reallocation will be required. Gets reset to false after every draw. */
	bool meshWasRecreated;

};