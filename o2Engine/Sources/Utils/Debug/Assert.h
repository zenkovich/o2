#pragma once

namespace o2
{
// Outs assert with description, if x is false
#define Assert(x, desc)                     \
	if (!(x))                               \
	{                                       \
	ErrorMessage(desc, __FILE__, __LINE__); \
	_asm{ int 3 };                          \
}

	void ErrorMessage(const char* desc, const char* file, long line);
}
