#pragma once

#include "../../Engine/EngineCore.h"
#include "../../Graphics/OpenGL/Buffers/PersistentMappedTripleVbo.h"
#include "../../Graphics/OpenGL/Buffers/PersistentMappedTripleIbo.h"

class Player;
class Shader;
class VertexArrayObject;
class Chunk;
class VertexBufferObject;
class IndexBufferObject;

class ChunkRenderer
{
public:

	ChunkRenderer();

	glm::vec3 GetOffsetForChunkDraw(const Chunk* chunk) const;

	void SetShaderChunkOffset(glm::vec3 chunkOffset);
	void SetShaderCameraMVP(const glm::mat4& cameraMVP);

	void Bind();

	void BindBlocksVertexBufferObject(VertexBufferObject* vbo);

	void Draw(VertexBufferObject* vbo, IndexBufferObject* ibo);

	void ReserveVbosAndIbosForChunkQuantity(uint32 chunksNum);

private:

	Shader* shader;

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

};