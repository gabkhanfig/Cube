#pragma once

#include <assert.h>
#include "Development.h"

//#ifdef DEVELOPMENT

/* Check a runtime condition, throwing an assertion if it fails. 
@param condition: Condition to check. Expected to be true. */
#define check(condition) assert(condition)

/* Check a runtime condition, throwing an assertion with a message if it fails. 
@param condition: Condition to check. Expected to be true.
@param message: The message to display if the assertion fails. */
#define checkm(condition, message) assert(condition && message)

//#else
//
///* Check assertion macro is disabled in non-development configuration. */
//#define check(condition)
//
///* Check with message assertion macro is disabled in non-development configuration. */
//#define checkm(condition, message)
//
//#endif