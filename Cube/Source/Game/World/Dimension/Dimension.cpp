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

	//Benchmark* chunkSpawning = Benchmark::StartBenchmark("Spawning a lot of chunks");
	//const int xdim = 1;
	//const int ydim = 0;
	//const int zdim = 1;

	//for (int x = 0; x < xdim; x++) {
	//	for (int y = -1; y < ydim; y++) {
	//		for (int z = 0; z < zdim; z++) {
	//			const ChunkPosition pos = { x, y, z };
	//			loadedChunks.insert({ pos, Chunk::LoadChunk(this, pos)});
	//		}
	//	}
	//}
	////LoadChunksAroundPlayer();
	//chunkSpawning->EndBenchmark();

	//Benchmark* chunkLoading = Benchmark::StartBenchmark("Loading a lot of chunks");
	//for (auto chunkPair : loadedChunks) {
	//	chunkPair.second->Init();
	//}
	//chunkLoading->EndBenchmark();

	GenerateMap();

	//RemoveDistantChunks();
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
	Print("going to remove da chunks");
	std::vector<ChunkPosition> removedChunks;
	std::vector<Chunk*> meshRegenChunks;

	const glm::dvec3 playerPos = Player::GetPlayer()->GetPosition();
	const ChunkPosition playerChunk = { playerPos.x / 16.0, playerPos.y / 16.0, playerPos.z / 16.0 };

	int i = 0;
	for (auto& chunkPair : loadedChunks) {
		Chunk* chunk = chunkPair.second;
		const ChunkPosition chunkPos = chunk->position;

		const ChunkPosition chunkOffset = { chunkPos.x - playerChunk.x, chunkPos.y - playerChunk.y, chunkPos.z - playerChunk.z };
		const int64 sqrDist = int64(chunkOffset.x * chunkOffset.x) + int64(chunkOffset.y * chunkOffset.y) + int64(chunkOffset.z * chunkOffset.z);

		if (sqrDist > (chunkRenderDistance * chunkRenderDistance)) {
			//chunk->UnloadChunk();
			removedChunks.push_back(chunkPos);
		}
	}
	Print(ToString(removedChunks.size()) + " amount of chunks removed");
	for (const auto& removedChunk : removedChunks) {
		Chunk* chunk = GetChunk(removedChunk);
		loadedChunks.erase(removedChunk);
		//delete chunk;
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
		#undef REGEN_IF_HASNT
	}
	
}

void Dimension::LoadChunksAroundPlayer()
{
	const glm::dvec3 playerPos = Player::GetPlayer()->GetPosition();
	const ChunkPosition playerChunk = { playerPos.x / 16.0, playerPos.y / 16.0, playerPos.z / 16.0 };

	//Print("Player Chunk = " + ToString({ playerChunk.x, playerChunk.y, playerChunk.z }));

	const ChunkPosition start = { playerChunk.x - chunkRenderDistance, playerChunk.y - chunkRenderDistance, playerChunk.z - chunkRenderDistance };
	const ChunkPosition end = { playerChunk.x + chunkRenderDistance, playerChunk.y + chunkRenderDistance, playerChunk.z + chunkRenderDistance };

	std::vector<Chunk*> newChunks;
	newChunks.reserve(64);

	for (int64 x = start.x; x < end.x; x++) {
		for (int64 y = start.y; y < end.y; y++) {
			for (int64 z = start.z; z < end.z; z++) {
				const ChunkPosition pos = { x, y, z };
				const int64 sqrDist = 
					int64((x - playerChunk.x) * (x - playerChunk.x)) 
					+ int64((y - playerChunk.y) * (y - playerChunk.y)) +
					int64((z - playerChunk.z) * (z - playerChunk.z));

				if (sqrDist > (chunkRenderDistance * chunkRenderDistance)) continue;

				if (GetChunk(pos) != nullptr) continue;


				Chunk* newChunk = Chunk::LoadChunk(this, pos);
				loadedChunks.insert({ pos, newChunk });
				newChunks.push_back(newChunk);
			}
		}
	}

	std::vector<Chunk*> meshed;
	meshed.reserve(64);

	for (auto& chunk : newChunks) {
		ChunkPosition chunkPos = chunk->position;
		Chunk* self = chunk;
		Chunk* bottom = GetChunk({ chunkPos.x, chunkPos.y - 1, chunkPos.z });
		Chunk* north = GetChunk({ chunkPos.x, chunkPos.y, chunkPos.z + 1 });
		Chunk* east = GetChunk({ chunkPos.x - 1, chunkPos.y, chunkPos.z });
		Chunk* south = GetChunk({ chunkPos.x, chunkPos.y, chunkPos.z - 1 });
		Chunk* west = GetChunk({ chunkPos.x + 1, chunkPos.y, chunkPos.z });
		Chunk* top = GetChunk({ chunkPos.x, chunkPos.y + 1, chunkPos.z });

#define REGEN_IF_HASNT(chunk) \
		if(chunk) { \
			if (std::find(meshed.begin(), meshed.end(), chunk) == meshed.end()) { \
				chunk->RegenerateChunkMeshData(); \
				meshed.push_back(chunk); \
			} \
		}

		REGEN_IF_HASNT(self);
		REGEN_IF_HASNT(bottom);
		REGEN_IF_HASNT(north);
		REGEN_IF_HASNT(east);
		REGEN_IF_HASNT(south);
		REGEN_IF_HASNT(west);
		REGEN_IF_HASNT(top);
		#undef REGEN_IF_HASNT
	}

}

void Dimension::RemoveAndLoadChunksAroundPlayer()
{
	Print("remove and load");

	// Which chunk positions should be removed from the world.
	std::vector<ChunkPosition> removedChunkPoses;

	// The newly generated chunks.
	std::vector<Chunk*> newChunks;

	// Chunks that have had their meshes regenerated.
	std::vector<Chunk*> meshed;

	Chunk* soloChunk = nullptr;
	int i = 0;
	for (auto& chunk : loadedChunks) {
		soloChunk = chunk.second;
		i++;
		//Print(ToString(i));
	}
	const glm::dvec3 playerPos = Player::GetPlayer()->GetPosition();
	const ChunkPosition playerChunkPos = { (playerPos.x / 16.0) - 1, (playerPos.y / 16.0) - 1, (playerPos.z / 16.0) - 1 };
	ChunkPosition newPos = playerChunkPos;
	newPos.y--;
	
	loadedChunks.erase(soloChunk->position);
	soloChunk->UnloadChunk();
	Chunk* newChunk = Chunk::LoadChunk(this, newPos);
	loadedChunks.insert({ newPos, newChunk });
	Print("generating new mesh data for chunk at position " + ToString({ newPos.x, newPos.y, newPos.z }));
	newChunk->RegenerateChunkMeshData();

	/*
	const glm::dvec3 playerPos = Player::GetPlayer()->GetPosition();
	const ChunkPosition playerChunkPos = { playerPos.x / 16.0, playerPos.y / 16.0, playerPos.z / 16.0 };

	for (auto& chunkPair : loadedChunks) {
		Chunk* chunk = chunkPair.second;
		const ChunkPosition chunkPos = chunk->position;

		const ChunkPosition chunkOffset = { chunkPos.x - playerChunkPos.x, chunkPos.y - playerChunkPos.y, chunkPos.z - playerChunkPos.z };
		const int64 sqrDist = int64(chunkOffset.x * chunkOffset.x) + int64(chunkOffset.y * chunkOffset.y) + int64(chunkOffset.z * chunkOffset.z);

		if (sqrDist > (chunkRenderDistance * chunkRenderDistance)) {
			//chunk->UnloadChunk();
			removedChunkPoses.push_back(chunkPos);
		}
	}

	const ChunkPosition start = { playerChunkPos.x - chunkRenderDistance, playerChunkPos.y - chunkRenderDistance, playerChunkPos.z - chunkRenderDistance };
	const ChunkPosition end = { playerChunkPos.x + chunkRenderDistance, playerChunkPos.y + chunkRenderDistance, playerChunkPos.z + chunkRenderDistance };

	for (int64 x = start.x; x < end.x; x++) {
		for (int64 y = start.y; y < end.y; y++) {
			for (int64 z = start.z; z < end.z; z++) {
				const ChunkPosition pos = { x, y, z };
				const int64 sqrDist =
					int64((x - playerChunkPos.x) * (x - playerChunkPos.x))
					+ int64((y - playerChunkPos.y) * (y - playerChunkPos.y)) +
					int64((z - playerChunkPos.z) * (z - playerChunkPos.z));

				if (sqrDist > (chunkRenderDistance * chunkRenderDistance)) continue;

				if (GetChunk(pos) != nullptr) continue;

				Chunk* newChunk = Chunk::LoadChunk(this, pos);
				loadedChunks.insert({ pos, newChunk });
				newChunks.push_back(newChunk);
			}
		}
	}

	for (auto& chunkPos : removedChunkPoses) {
		Chunk* center = GetChunk(chunkPos);
		//center->UnloadChunk();
		//loadedChunks.erase(chunkPos);
		std::vector<Chunk*> adjacent = GetAdjacentChunks(chunkPos);

		for (auto& chunk : adjacent) {
			if (std::find(meshed.begin(), meshed.end(), chunk) == meshed.end()) {
					chunk->RegenerateChunkMeshData();
					meshed.push_back(chunk);
			}
		}
	}

	for (auto& newChunk : newChunks) {
		newChunk->RegenerateChunkMeshData();
		meshed.push_back(newChunk);
		const ChunkPosition chunkPos = newChunk->position;
		std::vector<Chunk*> adjacent = GetAdjacentChunks(chunkPos);

		for (auto& chunk : adjacent) {
			if (std::find(meshed.begin(), meshed.end(), chunk) == meshed.end()) {
				chunk->RegenerateChunkMeshData();
				meshed.push_back(chunk);
			}
		}
	}
	*/
}

std::vector<Chunk*> Dimension::GetAdjacentChunks(const ChunkPosition& chunkPos)
{
	std::vector<Chunk*> adjacent;
	adjacent.reserve(6);

	Chunk* bottom = GetChunk({ chunkPos.x, chunkPos.y - 1, chunkPos.z });
	Chunk* north = GetChunk({ chunkPos.x, chunkPos.y, chunkPos.z + 1 });
	Chunk* east = GetChunk({ chunkPos.x - 1, chunkPos.y, chunkPos.z });
	Chunk* south = GetChunk({ chunkPos.x, chunkPos.y, chunkPos.z - 1 });
	Chunk* west = GetChunk({ chunkPos.x + 1, chunkPos.y, chunkPos.z });
	Chunk* top = GetChunk({ chunkPos.x, chunkPos.y + 1, chunkPos.z });

	if(bottom) adjacent.push_back(bottom);
	if(north) adjacent.push_back(north);
	if(east) adjacent.push_back(east);
	if(south) adjacent.push_back(south);
	if(west) adjacent.push_back(west);
	if(top) adjacent.push_back(top);
		
	return adjacent;
}

void Dimension::GenerateMap()
{
	const int xdim = 1;
	const int ydim = 1;
	const int zdim = 1;

	Benchmark* chunkData = Benchmark::StartBenchmark("generating chunk data");
	for (int x = 0; x < xdim; x++) {
		for (int y = 0; y < ydim; y++) {
			for (int z = 0; z < zdim; z++) {
				const ChunkPosition pos = { x, y, z };
				Chunk* chunk = Chunk::LoadChunk(this, pos);
				loadedChunks.insert({ pos, chunk });
			}
		}
	}
	chunkData->EndBenchmark();

	Benchmark* chunkInit = Benchmark::StartBenchmark("initializing chunks");
	for (auto& chunkPair : loadedChunks) {
		chunkPair.second->Init();
	}
	chunkInit->EndBenchmark();
}
