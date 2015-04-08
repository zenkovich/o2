#pragma once

#include "public_namespace.h"

OPEN_O2_NAMESPACE

#ifdef PLATFORM_WIN
#	define IsWinPlatform() true
#	define IsMacPlatform() false
#	define IsIOSPlatform() false
#	define IsPCPlatform() true
#	define IsTabletPlatform() false
#	define IsPhonePlatform() false
#endif

CLOSE_O2_NAMESPACE
