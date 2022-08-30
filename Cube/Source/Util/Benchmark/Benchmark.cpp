#include "Benchmark.h"

Benchmark* Benchmark::StartBenchmark(const std::string& benchmarkName)
{
	Benchmark* benchmark = new Benchmark();
	benchmark->name = benchmarkName;
	Print("[Benchmark]: Starting benchmark \"" + benchmarkName + "\"");
	benchmark->start = std::chrono::high_resolution_clock::now();
	return benchmark;
}

double Benchmark::EndBenchmark()
{
	end = std::chrono::high_resolution_clock::now();
	double benchmarkEnd = std::chrono::duration<double, std::milli>(end - start).count();
	Print("[Benchmark]: Benchmark \"" + name + "\" executed in " + ToString(benchmarkEnd) + " ms.");
	delete this;
	return benchmarkEnd;
}
