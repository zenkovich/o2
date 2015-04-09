#include "ConsoleLogStream.h"

#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <Windows.h>
#include <wincon.h>

namespace o2
{
	ConsoleLogStream::ConsoleLogStream():
		LogStream()
	{
		InitConsole();
	}

	ConsoleLogStream::ConsoleLogStream(const std::string& id):
		LogStream(id)
	{
		InitConsole();
	}

	ConsoleLogStream::~ConsoleLogStream()
	{
		FreeConsole();
	}

	void ConsoleLogStream::OutStrEx(const std::string& str)
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
}