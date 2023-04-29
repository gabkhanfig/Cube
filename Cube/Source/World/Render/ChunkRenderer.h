#pragma once

#include "../../Engine/EngineCore.h"
#include "../../Graphics/Geometry/ChunkMesh.h"
#include "../../Graphics/OpenGL/Buffers/PersistentMappedTripleVbo.h"
#include "../../Graphics/OpenGL/Buffers/PersistentMappedTripleIbo.h"
#include "../../Graphics/OpenGL/Buffers/PersistentMappedTripleIndirect.h"
#include "../../Graphics/OpenGL/Buffers/PersistentMappedTripleBuffer.h"
#include "../WorldTransform.h"
#include "../Chunk/ChunkDataTypes.h"

class Player;
class Shader;
class VertexArrayObject;
class Chunk;
class VertexBufferObject;
class IndexBufferObject;

class ChunkRenderer
{
	struct DrawCallData {
		glm::dvec3 playerPos;
		glm::mat4 cameraMVP;
		bool canDraw;
		DrawCallData() : canDraw(false) {}
	};

	struct ChunkBuffers {
		PersistentMappedTripleBuffer<VertexBufferObject, BlockQuad>* vbos;
		PersistentMappedTripleBuffer<IndexBufferObject, uint32>* ibos;
		ChunkMesh* mesh;
		glm::vec3 positionOffset;
	};

	struct ChunkDrawCall {
		Chunk* chunk;
		PersistentMappedTripleBuffer<VertexBufferObject, BlockQuad>* vbos;
		PersistentMappedTripleBuffer<IndexBufferObject, uint32>* ibos;
		glm::vec3 positionOffset;
	};

public:

	ChunkRenderer();

	glm::vec3 GetOffsetForChunkDraw(const Chunk* chunk) const;

	void SetShaderChunkOffset(glm::vec3 chunkOffset);

	void SetShaderCameraMVP(const glm::mat4& cameraMVP);

	void Bind();

	void BindBlocksVertexBufferObject(VertexBufferObject* vbo);

	void Draw(VertexBufferObject* vbo, IndexBufferObject* ibo);

	void ReserveVbosAndIbosForChunkQuantity(uint32 chunksNum);

	PersistentMappedTripleVbo<BlockQuad>* GetMultidrawVbos() const { return multidrawVbos; }

	PersistentMappedTripleIbo* GetMultidrawIbos() const { return multidrawIbos; }

	void SwapNextBuffers();

	void DrawAllChunks(const HashMap<ChunkPosition, Chunk*>& chunks);

	void MultidrawIndirectAllChunks(const HashMap<ChunkPosition, Chunk*>& chunks);

	void StoreModifyDrawCallData();

	ChunkMesh* GetChunkMesh(Chunk* chunk) const;

	/* Will create new mesh objects for the provided chunks in the meshes map. */
	void AllocateMeshesForChunks(const HashMap<ChunkPosition, Chunk*>& chunks);
	/* Will create a mesh object for the provided chunk in the meshes map. */
	void AllocateMeshForChunk(Chunk* chunk);

	void DrawAllChunksAndPrepareNext();

	void SetRemeshedChunks(const darray<Chunk*> newRemeshedChunks);

private:

	void PerformBoundDrawCalls();

	glm::vec3 GetChunkShaderPositionOffset(const glm::dvec3 playerPos, const Chunk* chunk);

private:

	Shader* shader;

	Shader* multidrawShader;

	VertexArrayObject* vao;

	const GlobalString cameraMvpUniform;

	const GlobalString chunkOffsetUniform;

	/* Persistently mapped triple buffered VBOs for faster gpu data syncronization and multidraw.
	This will have a capacity (like a dynamic array) that should be able to hold more than or equal to the amount of quads required by the mesh.
	This means upon remeshing, the existing VBOs will likely be able to be used. If not, the VBOs will need to be reallocated. */
	PersistentMappedTripleVbo<BlockQuad>* multidrawVbos;

	/* Persistently mapped triple buffered IBOs for faster gpu data syncronization and multidraw.
	This will have a capacity (like a dynamic array) that should be able to hold more than or equal to the amount of indices required by the mesh.
	This means upon remeshing, the existing IBOs will likely be able to be used. If not, the IBOs will need to be reallocated. */
	PersistentMappedTripleIbo* multidrawIbos;

	/**/
	PersistentMappedTripleVbo<glm::vec3>* multidrawOffsets;

	PersistentMappedTripleIndirect* multidrawIndirectBuffers;

	HashMap<Chunk*, ChunkBuffers> buffers;

	//HashMap<Chunk*, ChunkRenderMeshData*> drawChunks;

	DrawCallData drawCalls[2];
	int boundDrawCallId;
	int modifyDrawCallId;

	PersistentMappedTripleVbo<BlockQuad>* vbos;
	PersistentMappedTripleIbo* ibos;


	darray<Chunk*> remeshedChunks;

	darray<ChunkDrawCall> frameChunkDrawCalls;
};