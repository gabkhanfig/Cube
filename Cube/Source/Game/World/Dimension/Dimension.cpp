#include "Dimension.h"
#include <Game/World/Chunk/Chunk.h>
#include <Game/World/Block/Block.h>
#include <Util/Benchmark/Benchmark.h>

Dimension::Dimension()
{
}

void Dimension::InitChunks()
{
	/*
	* 1 chunk : 66MB
	* 2 chunk : 67MB
	* 3 chunk : 68MB
	* 10 chunk : 72MB
	* 100 chunk : 126MB
	* 1000 chunk : 632MB
	*/

	Block::SetupSingletonTest();

	Benchmark* chunkSpawning = Benchmark::StartBenchmark("Spawning a lot of chunks");
	const int xdim = 1;
	const int ydim = 10;
	const int zdim = 1;

	for (int x = 0; x < xdim; x++) {
		for (int y = 0; y < ydim; y++) {
			for (int z = 0; z < zdim; z++) {
				ChunkPosition pos = { x, y, z };
				Chunk* chunk = new Chunk(pos);
				loadedChunks.insert({ pos, chunk });
			}
		}
	}
	chunkSpawning->EndBenchmark();

	Benchmark* chunkLoading = Benchmark::StartBenchmark("Loading a lot of chunks");
	for (auto chunkPair : loadedChunks) {
		chunkPair.second->Init();
	}
	chunkLoading->EndBenchmark();
}

void Dimension::Tick(float deltaTime)
{
	for (auto chunkPair : loadedChunks) {
		Chunk* chunk = chunkPair.second;
		chunk->DrawChunk();
	}
}

Chunk* Dimension::GetChunk(const ChunkPosition& position)
{
	if (loadedChunks.find(position) != loadedChunks.end()) {
		return loadedChunks.at(position);
	}
	return nullptr;
}
