#pragma once

#include "../EngineTypes.h"

class Logger
{
public:

	/* Logs a message and adds a newline. */
	static void Log(const String& message);
};

/* See Logger::Log(). */
forceinline void cubeLog(const String& message) {
	Logger::Log(message);
}