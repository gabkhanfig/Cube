#pragma once

/* Length of a chunk (applies to width and height as well). 
From brief benchmarking, this seems to be a reasonable chunk size to not take too long to remesh. Done on a Ryzen 9 7950x */
#define CHUNK_LENGTH 32

/* Amount of blocks per chunk */
#define CHUNK_SIZE (CHUNK_LENGTH * CHUNK_LENGTH * CHUNK_LENGTH)

