#pragma once

#include <string>
#include <cstdarg>

namespace o2
{
	typedef std::string String;

	String FormatStr(const char* format, ...);
	String FormatStr(const char* format, va_list vlist);
}