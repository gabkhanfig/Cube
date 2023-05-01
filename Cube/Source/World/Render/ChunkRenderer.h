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

	void SwapNextBuffers();

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

	void DrawChunk(const ChunkDrawCall& drawCall);

	glm::vec3 GetChunkShaderPositionOffset(const glm::dvec3 playerPos, const Chunk* chunk);

private:

	Shader* shader;

	VertexArrayObject* vao;

	const GlobalString cameraMvpUniform;

	const GlobalString chunkOffsetUniform;

	HashMap<Chunk*, ChunkBuffers> buffers;

	DrawCallData drawCalls[2];
	int boundDrawCallId;
	int modifyDrawCallId;


	darray<Chunk*> remeshedChunks;

	darray<ChunkDrawCall> frameChunkDrawCalls;
};