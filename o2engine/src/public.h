#ifndef PUBLIC_H
#define PUBLIC_H

/* 
 * Engine public file.
 * Contains basic stuff using in engine. 
 */

#include "engine_settings.h"

/* Engine namespace macros. */
#ifdef ENGINE_NAMESPACE_NAME

	/** Opening engine namespace macros. */
	#define OPEN_O2_NAMESPACE namespace ENGINE_NAMESPACE_NAME {

	/** Closing engine namespace macros.  */
	#define CLOSE_O2_NAMESPACE }

#else

	/** Opening engine namespace macros. */
	#define OPEN_O2_NAMESPACE

	/** Closing engine namespace macros.  */
	#define CLOSE_O2_NAMESPACE

#endif //ENGINE_NAMESPACE_NAME

//some includes
#include "util/assert.h"
#include <string>

//types
typedef unsigned long long uint64;
typedef unsigned int       uint32;
typedef unsigned short     uint16;
typedef unsigned char      uint8;
typedef long long          int64;

//some macros
#ifdef PLATFORM_WIN
#	define isWinPlatform() true
#	define isMacPlatform() false
#	define isIOSPlatform() false
#	define isPCPlatform() true
#	define isTabletPlatform() false
#	define isPhonePlatform() false
#endif

//add windows libraries
#if defined(PLATFORM_WIN) && defined(RENDER_OGL)
#	pragma comment(lib, "opengl32.lib")
#	pragma comment(lib, "glu32.lib")
#	pragma comment(lib, "odbc32.lib")
#	pragma comment(lib, "odbccp32.lib")
#elif defined(PLATFORM_WIN) && defined(RENDER_D3D9C)
#	pragma comment(lib, __FILE__ "/../../dependencies/dx9c/Lib/d3d9.lib") // aww eeaaah bad ass combination
#	pragma comment(lib, __FILE__ "/../../dependencies/dx9c/Lib/d3dx9.lib")
#	pragma comment(lib, __FILE__ "/../../dependencies/pthreads/pthreadVC2.lib")
#endif

#include "util/mem_utils/memory_manager.h"
#include "util/mem_utils/alloc_operators.h"

#endif //PUBLIC_H