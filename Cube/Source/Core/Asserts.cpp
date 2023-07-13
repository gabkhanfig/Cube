#include "Asserts.h"

void _CheckImplementation(bool condition, const String& message, const char* file, int line)
{
	if (condition) return;
	std::cout << "[CHECK FAILED]:\n"
		<< message
		<< "\nFile: " << file
		<< "\nLine: " << line;
	DebugBreak();
	//abort();
}
