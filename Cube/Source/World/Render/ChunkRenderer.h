#pragma once

#include "../../Engine/EngineCore.h"

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

private:

	Shader* shader;

	VertexArrayObject* vao;

	const GlobalString cameraMvpUniform;

	const GlobalString chunkOffsetUniform;

};