#pragma once

#include "../EngineTypes.h"

class Logger
{
public:

	/* Logs a message and adds a newline. */
	static void Log(const string& message);
};

/* See Logger::Log(). */
forceinline void cubeLog(const string& message) {
	Logger::Log(message);
}