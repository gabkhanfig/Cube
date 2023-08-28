#pragma once

#include "../../Engine/EngineCore.h"
#include "../../Graphics/Geometry/ChunkMesh.h"
#include "../../Graphics/OpenGL/Buffers/PersistentMappedTripleBuffer.h"
#include "../WorldTransform.h"
#include "../Chunk/ChunkDataTypes.h"
#include "../Chunk/ChunkDrawCall.h"

class Player;
class Shader;
class RasterShader;
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

	//struct ChunkBuffers {
	//	PersistentMappedTripleBuffer<VertexBufferObject, BlockQuad>* vbos;
	//	PersistentMappedTripleBuffer<IndexBufferObject, uint32>* ibos;
	//	ChunkMesh* mesh;
	//};

public:

	ChunkRenderer();

	void SetShaderChunkOffset(glm::vec3 chunkOffset);

	void SetShaderCameraMVP(const glm::mat4& cameraMVP);

	void Bind();

	void SwapNextBuffers();

	void StoreModifyDrawCallData();

	void DrawAllChunksAndPrepareNext(darray<ChunkDrawCall> chunksToDrawNextFrame);

	void SetRemeshedChunks(const darray<Chunk*>& newRemeshedChunks);

	void RemoveChunkFromFrameDraw(Chunk* chunk);

	void CreateGLBuffersForChunks(const darray<Chunk*>& chunks);

	static glm::vec3 GetChunkShaderPositionOffset(const glm::dvec3 playerPos, const Chunk* chunk);

private:

	void PerformBoundDrawCalls();

	void DrawChunk(const Chunk* drawChunk);

private:

	RasterShader* blockShader;

	VertexArrayObject* vao;

	IndexBufferObject* blocksIbo;

	const GlobalString cameraMvpUniform;

	const GlobalString chunkOffsetUniform;

	//HashMap<Chunk*, ChunkBuffers> buffers;

	DrawCallData drawCalls[2];
	int boundDrawCallId;
	int modifyDrawCallId;

	darray<Chunk*> remeshedChunks;

	darray<ChunkDrawCall> frameChunkDrawCalls;

	darray<ChunkDrawCall> drawCallsToExecute;
};