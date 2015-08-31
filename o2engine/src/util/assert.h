#pragma once

#include "public_namespace.h"
OPEN_O2_NAMESPACE

#ifdef assert
#undef assert
#endif

/* Outing assert with description, if x is false. */
#define o2assert(x, desc) \
if (!(x)) \
{ \
	ErrorMessage(desc, __FILE__, __LINE__); \
	_asm{ int 3 }; \
}

void ErrorMessage(const char* desc, const char* file, long line);

CLOSE_O2_NAMESPACE