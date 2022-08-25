#include "Benchmark.h"

Benchmark* Benchmark::StartBenchmark(const std::string& benchmarkName)
{
	Benchmark* benchmark = new Benchmark();
	benchmark->name = benchmarkName;
	Print("[Benchmark]: Starting benchmark \"" + benchmarkName + "\"");
	benchmark->start = std::chrono::high_resolution_clock::now();
	return benchmark;
}

double Benchmark::EndBenchmark(Benchmark* benchmark)
{
	benchmark->end = std::chrono::high_resolution_clock::now();
	double benchmarkEnd = std::chrono::duration<double, std::milli>(benchmark->end - benchmark->start).count();
	Print("[Benchmark]: Benchmark \"" + benchmark->name + "\" executed in " + ToString(benchmarkEnd) + " ms.");
	delete benchmark;
	return benchmarkEnd;
}
