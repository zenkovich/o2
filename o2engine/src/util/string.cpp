#include "string.h"

#include <cstdarg>

OPEN_O2_NAMESPACE

std::string format( const char* str, ... )
{
	const uint32 bufsz = 1096;
	char buf[bufsz];

	va_list ap;
	va_start(ap, str);
	vsnprintf(buf, bufsz, str, ap);
	va_end(ap);

	return std::string(buf);
}

CLOSE_O2_NAMESPACE