#pragma once

#include <Development.h>
#include <Engine/EngineCore.h>
#include <Util/String/StringUtil.h>
#include <Util/Logging/Log.h>

#define check(condition)
#define checkLog(condition, message)

#ifdef DEVELOPMENT
#define check(condition) assert(condition)
#define checkLog(condition, message) assert(condition && message);
#endif