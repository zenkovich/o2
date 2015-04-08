#include "console_log_stream.h"

#include <Windows.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <wincon.h>

OPEN_O2_NAMESPACE

ConsoleLogStream::ConsoleLogStream(uint8 level):
LogStream()
{
	SetLevel(level);
	if (level > 0)
		InitConsole();
}

ConsoleLogStream::ConsoleLogStream(const String& id, uint8 level):
LogStream(id)
{
	SetLevel(level);
	if (level > 0)
		InitConsole();
}

ConsoleLogStream::~ConsoleLogStream()
{
	FreeConsole();
}

void ConsoleLogStream::OutStrEx(const String& str)
{
	printf(str.c_str());
	printf("\n");
}

void ConsoleLogStream::InitConsole()
{
	if (AllocConsole())
	{
		int hCrt = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
		*stdout = *(::_fdopen(hCrt, "w"));
		::setvbuf(stdout, NULL, _IONBF, 0);
		*stderr = *(::_fdopen(hCrt, "w"));
		::setvbuf(stderr, NULL, _IONBF, 0);
	}
}

CLOSE_O2_NAMESPACE