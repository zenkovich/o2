#pragma once

#include <string>
#include <cstdarg>

namespace o2
{
	typedef std::string String;

	String FormatStr(const String& format, ...);
	String FormatStr(const String& format, va_list vlist);
}