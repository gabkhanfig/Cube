#include "ChunkRender.h"
#include "Chunk.h"
#include <Engine/Render/Buffers/IndexBuffer.h>
#include <Engine/Render/Buffers/VertexBuffer.h>
#include <Engine/Render/Buffers/VertexArray.h>
#include <Engine/Render/Buffers/VertexBufferLayout.h>
#include <Engine/Render/Renderer.h>
#include <Engine/Render/Shader/Shader.h>
#include <Game/World/Block/Block.h>
#include <Util/Benchmark/Benchmark.h>

void ChunkBuffers::Bind()
{
	vbo->Bind();
	ibo->Bind();
	vao->Bind();
}

void ChunkBuffers::MakeNewBlockBuffers(const std::vector<BlockQuad>& quads)
{
	vao = new VertexArray();
	vbo = new VertexBuffer(&quads[0], sizeof(BlockQuad) * quads.size());

	vao->AddBuffer(vbo, VertexBufferLayout::MakeVertexBufferLayoutForBlock());

	ibo = IndexBuffer::MakeQuadsIndexBuffer(quads.size());

	quadsToDraw = quads.size();
}

ChunkRender::ChunkRender(Chunk* _owner)
	: owner(_owner)
{
}

void ChunkRender::Draw()
{
	if (!canRender) return;

	glm::vec3 shiftChunkPos;
	const bool chunkVisible = owner->IsChunkInCamera(shiftChunkPos);
	if (!chunkVisible) return;

	graphicsBuffers.Bind();
	Shader::GetBoundShader()->SetUniform3float("u_ChunkPosition", shiftChunkPos);
	Renderer::DrawQuads(graphicsBuffers.quadsToDraw);
}

void ChunkRender::ConstructBlockMesh()
{
	Benchmark* construct = Benchmark::StartBenchmark("construct chunk mesh on cpu");
	constexpr size_t quadsReserve = 256;

	graphicsBuffers.ClearBuffers();

	if (owner->blocks.GetBlocksMode() == ChunkBlocks::BlocksMode::AirFilled) {
		canRender = false;
		return;
	}

	std::vector<BlockQuad> quads;
	quads.reserve(quadsReserve);

	BlockQuad quadBuffer[6];
	uint32 generatedQuads;

	for (int i = 0; i < CHUNK_SIZE; i++) {
		Block* block = owner->blocks.GetBlock(i);

		const BlockPosition relativeLocation = BlockPosition::FromBlockIndex(i);
		const WorldPosition worldPos = WorldPosition::ToWorldPosition(owner->position, relativeLocation);

		if (block->GenerateBlockQuads(owner, worldPos, { relativeLocation, 0, 0 }, quadBuffer, generatedQuads)) {
			quads.insert(quads.end(), quadBuffer, quadBuffer + generatedQuads);
		}
	}
	
	if (quads.size() == 0) {
		canRender = false;
		return;
	}
	construct->EndBenchmark();

	Benchmark* buffers = Benchmark::StartBenchmark("load data to gpu");
	graphicsBuffers.MakeNewBlockBuffers(quads);
	canRender = true;
	buffers->EndBenchmark();
}

void ChunkRender::AsyncConstructBlockMesh()
{
	ConstructBlockMesh();
}


