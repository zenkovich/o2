#include "String.h"

namespace o2
{
	char charBuf[4096];

	o2::String FormatStr(const char* format, ...)
	{
		va_list vlist;
		va_start(vlist, format);

		vsprintf(charBuf, format, vlist);

		va_end(vlist);

		return (String)charBuf;
	}

	o2::String FormatStr(const char* format, va_list vlist)
	{
		vsprintf(charBuf, format, vlist);
		return (String)charBuf;
	}

}