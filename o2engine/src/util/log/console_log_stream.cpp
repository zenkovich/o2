#include "console_log_stream.h"

#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <Windows.h>
#include <wincon.h>

OPEN_O2_NAMESPACE

cConsoleLogStream::cConsoleLogStream(uint8 level):
	cLogStream()
{
	setLevel(level);
	if (level > 0)
		initConsole();
}

cConsoleLogStream::cConsoleLogStream( const std::string& id, uint8 level ):
	cLogStream(id)
{
	setLevel(level);
	if (level > 0)
		initConsole();
}

cConsoleLogStream::~cConsoleLogStream()
{
	FreeConsole();
}

void cConsoleLogStream::outStrEx( const std::string& str )
{
	printf(str.c_str());
	printf("\n");	
}

void cConsoleLogStream::initConsole()
{
	if ( AllocConsole() )
	{
		int hCrt = _open_osfhandle((long)
		GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
		*stdout = *(::_fdopen(hCrt, "w"));
		::setvbuf(stdout, NULL, _IONBF, 0);
		*stderr = *(::_fdopen(hCrt, "w"));
		::setvbuf(stderr, NULL, _IONBF, 0);
    }
}

CLOSE_O2_NAMESPACE