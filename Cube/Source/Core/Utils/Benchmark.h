#pragma once

#include <chrono>

enum class TimeUnit {
	nanosecond,
	microsecond,
	millisecond,
	second
};

inline double TimeDurationToDisplayDuration(const std::chrono::steady_clock::time_point& start, const std::chrono::steady_clock::time_point& end, const TimeUnit timeUnit) {
	const auto duration = end - start;
	double durationCast = 0;
	switch (timeUnit) {
	case TimeUnit::nanosecond:
		durationCast = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count());
		break;
	case TimeUnit::microsecond:
		durationCast = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count()) / 1000.0;
		break;
	case TimeUnit::millisecond:
		durationCast = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(duration).count()) / 1000.0;
		break;
	case TimeUnit::second:
		durationCast = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(duration).count()) / 1000.0;
		break;
	}
	return durationCast;
}

inline const char* TimeUnitToString(const TimeUnit timeUnit) {
	switch (timeUnit) {
	case TimeUnit::nanosecond:
		return "ns";
	case TimeUnit::microsecond:
		return "us";
	case TimeUnit::millisecond:
		return "ms";
	case TimeUnit::second:
		return "s";
	}
}

#define BENCHMARK_START(benchmarkName)\
const auto benchmarkName##_start = std::chrono::high_resolution_clock::now()

#define BENCHMARK_END(benchmarkName, timeUnit)\
const auto benchmarkName##_end = std::chrono::high_resolution_clock::now();\
std::cout << "[BENCHMARK] " STR(benchmarkName) ": " << TimeDurationToDisplayDuration(benchmarkName##_start, benchmarkName##_end, timeUnit) << TimeUnitToString(timeUnit) << '\n'



