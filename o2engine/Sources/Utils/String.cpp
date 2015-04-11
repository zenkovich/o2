#include "String.h"

namespace o2
{
	char charBuf[4096];

	o2::String FormatStr(const String& format, ...)
	{
		va_list vlist;
		va_start(vlist, format);

		vsprintf(charBuf, format.c_str(), vlist);

		va_end(vlist);

		return (String)charBuf;
	}

	o2::String FormatStr(const String& format, va_list vlist)
	{
		vsprintf(charBuf, format.c_str(), vlist);
		return (String)charBuf;
	}

}