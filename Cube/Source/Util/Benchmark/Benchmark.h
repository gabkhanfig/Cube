#pragma once

#include <CubeCore.h>
#include <chrono>

class Benchmark 
{
private:

	std::string name;

	std::chrono::steady_clock::time_point start;

	std::chrono::steady_clock::time_point end;

public:

	static Benchmark* StartBenchmark(const std::string& benchmarkName);

	static double EndBenchmark(Benchmark* benchmark);
};