#pragma once

#undef NDEBUG

#include <assert.h>
#include "EngineTypes.h"
#include "Development.h"

void _CheckImplementation(bool condition, const string& message, const char* file, int line);

#define _str(s) #s

#ifdef DEVELOPMENT

/* Check a runtime condition, throwing an assertion if it fails. 
@param condition: Condition to check. Expected to be true. */
#define check(condition) _CheckImplementation(condition, _str(condition), __FILE__, __LINE__)

/* Check a runtime condition, throwing an assertion with a message if it fails. 
@param condition: Condition to check. Expected to be true.
@param message: The message to display if the assertion fails. */
#define checkm(condition, message) _CheckImplementation(condition, string(_str(condition)) + " \"" + message + "\"", __FILE__, __LINE__)



#define checkNotNull(_prt) _CheckImplementation((_prt != nullptr), _str(_ptr), __FILE__, __LINE__)

#else

/* Check assertion macro is disabled in non-development configuration. */
#define check(condition)

/* Check with message assertion macro is disabled in non-development configuration. */
#define checkm(condition, message)

/* Check if pointer is non-null is disabled in non-development configuration. */
#define checkNotNull(_prt)

#endif

//#undef _str