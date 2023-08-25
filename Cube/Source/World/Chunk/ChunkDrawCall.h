#pragma once

#include "../../Engine/EngineCore.h"

class Chunk;
class VertexBufferObject;
class IndexBufferObject;

struct ALIGN_AS(64) ChunkDrawCall
{
	Chunk* chunk;
	VertexBufferObject* vbo;
	IndexBufferObject* ibo;
	glm::vec3 chunkPositionOffset;
	uint32 indicesToDraw;
};