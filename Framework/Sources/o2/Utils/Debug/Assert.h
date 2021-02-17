#pragma once

namespace o2
{
// Outs assert with description, when true_condition is false
#ifdef PLATFORM_WINDOWS
#define Assert(true_condition, desc)                    \
    {                                                   \
	    if (!(true_condition))                          \
	    {                                               \
	        o2::ErrorMessage(desc, __FILE__, __LINE__); \
	        __debugbreak();                             \
        }                                               \
	}
#else
#define Assert(true_condition, desc)                    \
	{												    \
		if (!(true_condition))                          \
		{                                               \
		    o2::ErrorMessage(desc, __FILE__, __LINE__); \
		}												\
	}
#endif
	void ErrorMessage(const char* desc, const char* file, long line);
}
