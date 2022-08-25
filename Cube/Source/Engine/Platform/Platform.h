#pragma once

#define WINDOWS_PLATFORM_X64 1
#define PLATFORM 0

#if _WIN64 == 1
#define PLATFORM WINDOWS_PLATFORM_X64
#endif