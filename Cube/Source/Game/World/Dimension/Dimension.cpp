#include "Dimension.h"
#include <Game/World/Chunk/Chunk.h>
#include <Game/World/Block/Block.h>
#include <Util/Benchmark/Benchmark.h>
#include <Game/World/Terrain/Biome/Biome.h>
#include <Game/Player/Player.h>

Dimension::Dimension()
{
	biome = new Biome();
	dimId = 0;
}

void Dimension::InitChunks()
{
	Block::SetupSingletonTest();

	Benchmark* chunkSpawning = Benchmark::StartBenchmark("Spawning a lot of chunks");
	const int xdim = 1;
	const int ydim = 0;
	const int zdim = 1;

	//for (int x = 0; x < xdim; x++) {
	//	for (int y = -1; y < ydim; y++) {
	//		for (int z = 0; z < zdim; z++) {
	//			const ChunkPosition pos = { x, y, z };
	//			loadedChunks.insert({ pos, Chunk::LoadChunk(this, pos)});
	//		}
	//	}
	//}
	LoadChunksAroundPlayer();
	chunkSpawning->EndBenchmark();

	Benchmark* chunkLoading = Benchmark::StartBenchmark("Loading a lot of chunks");
	for (auto chunkPair : loadedChunks) {
		chunkPair.second->Init();
	}
	chunkLoading->EndBenchmark();

	RemoveDistantChunks();
}

void Dimension::Tick(float deltaTime)
{
	//RemoveDistantChunks();

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

Block* Dimension::GetBlockForWorldPosition(const WorldPosition& worldPos)
{
	return biome->GetBlockForPos(worldPos);
}

void Dimension::RemoveDistantChunks()
{
	std::vector<ChunkPosition> removedChunks;
	std::vector<Chunk*> meshRegenChunks;

	const glm::dvec3 playerPos = Player::GetPlayer()->GetPosition();
	const ChunkPosition playerChunk = { playerPos.x / 16.0, playerPos.y / 16.0, playerPos.z / 16.0 };

	for (auto& chunkPair : loadedChunks) {
		Chunk* chunk = chunkPair.second;
		const ChunkPosition chunkPos = chunk->position;

		const ChunkPosition chunkOffset = { chunkPos.x - playerChunk.x, chunkPos.y - playerChunk.y, chunkPos.z - playerChunk.z };
		const int64 sqrDist = int64(chunkOffset.x * chunkOffset.x) + int64(chunkOffset.y * chunkOffset.y) + int64(chunkOffset.z * chunkOffset.z);
		//Print(ToString(sqrDist) + " squared distance found");
		if (sqrDist > (chunkRenderDistance * chunkRenderDistance)) {
			//Print("chunk is out of range... unloading");
			chunk->UnloadChunk();
			loadedChunks.erase(chunkPos);
			removedChunks.push_back(chunkPos);
		}
	}

	for (auto& chunkPos : removedChunks) {
		Chunk* bottom = GetChunk({ chunkPos.x, chunkPos.y - 1, chunkPos.z });
		Chunk* north = GetChunk({ chunkPos.x, chunkPos.y, chunkPos.z + 1 });
		Chunk* east = GetChunk({ chunkPos.x - 1, chunkPos.y, chunkPos.z });
		Chunk* south = GetChunk({ chunkPos.x, chunkPos.y, chunkPos.z - 1 });
		Chunk* west = GetChunk({ chunkPos.x + 1, chunkPos.y, chunkPos.z });
		Chunk* top = GetChunk({ chunkPos.x, chunkPos.y + 1, chunkPos.z });

		#define REGEN_IF_HASNT(chunk) \
		if(chunk) { \
			if (std::find(meshRegenChunks.begin(), meshRegenChunks.end(), chunk) == meshRegenChunks.end()) { \
				chunk->RegenerateChunkMeshData(); \
				meshRegenChunks.push_back(chunk); \
			} \
		}
		
		REGEN_IF_HASNT(bottom);
		REGEN_IF_HASNT(north);
		REGEN_IF_HASNT(east);
		REGEN_IF_HASNT(south);
		REGEN_IF_HASNT(west);
		REGEN_IF_HASNT(top);
	}

	//for()
}

void Dimension::LoadChunksAroundPlayer()
{
	const glm::dvec3 playerPos = Player::GetPlayer()->GetPosition();
	const ChunkPosition playerChunk = { playerPos.x / 16.0, playerPos.y / 16.0, playerPos.z / 16.0 };

	const ChunkPosition start = { playerChunk.x - chunkRenderDistance, playerChunk.y - chunkRenderDistance, playerChunk.z - chunkRenderDistance };
	const ChunkPosition end = { playerChunk.x + chunkRenderDistance, playerChunk.y + chunkRenderDistance, playerChunk.z + chunkRenderDistance };

	for (int64 x = start.x; x < end.x; x++) {
		for (int64 y = start.y; y < end.y; y++) {
			for (int64 z = start.z; z < end.z; z++) {
				const ChunkPosition pos = { x, y, z };
				const int64 sqrDist = 
					int64((x - playerChunk.x) * (x - playerChunk.x)) 
					+ int64((y - playerChunk.y) * (y - playerChunk.y)) +
					int64((z - playerChunk.x) * (z - playerChunk.z));

				if (sqrDist > (chunkRenderDistance * chunkRenderDistance)) continue;

				loadedChunks.insert({ pos, Chunk::LoadChunk(this, pos) });
			}
		}
	}
}
