#pragma once

#include "Platform.h"

namespace DefaultPlatformTypes
{
	/* Signed 8 bit integer. Technically a char so watch string conversion. */
	typedef char int8;

	/* Signed 16 bit integer. */
	typedef short int16;

	/* Signed 64 bit integer. */
	typedef long long int64;

	/* Unsigned 8 bit integer. Technically a char so watch string conversion. */
	typedef unsigned char uint8;

	/* Unsigned 16 bit integer. */
	typedef unsigned short uint16;

	/* Unsigned 32 bit integer. */
	typedef unsigned int uint32;

	/* Unsigned 64 bit ingteger. */
	typedef unsigned long long uint64;
}

namespace WindowsPlatformTypes
{
	/* Signed 8 bit integer. Technically a char so watch string conversion. */
	typedef char int8;

	/* Signed 16 bit integer. */
	typedef short int16;

	/* Signed 64 bit integer. */
	typedef long long int64;

	/* Unsigned 8 bit integer. Technically a char so watch string conversion. */
	typedef unsigned char uint8;

	/* Unsigned 16 bit integer. */
	typedef unsigned short uint16;

	/* Unsigned 32 bit integer. */
	typedef unsigned int uint32;

	/* Unsigned 64 bit ingteger. */
	typedef unsigned long long uint64;
}

#if PLATFORM == WINDOWS_PLATFORM_X64
#define PLATFORM_TYPES WindowsPlatformTypes
#else
#define PLATFORM_TYPES DefaultPlatformTypes
#endif