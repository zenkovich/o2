#pragma once

namespace o2
{
// Outs assert with description, when x is false
#ifdef PLATFORM_WINDOWS
#define Assert(x, desc)                         \
	if (!(x))                                   \
	{                                           \
	    ErrorMessage(desc, __FILE__, __LINE__); \
	    _asm{ int 3 };                          \
    }
#else
#define Assert(x, desc)                         \
	if (!(x))                                   \
	{                                           \
	    ErrorMessage(desc, __FILE__, __LINE__); \
    }
#endif
	void ErrorMessage(const char* desc, const char* file, long line);
}
