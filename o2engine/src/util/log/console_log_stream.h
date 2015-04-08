#pragma once

#include "log_stream.h"

OPEN_O2_NAMESPACE

class ConsoleLogStream:public LogStream
{
public:
	ConsoleLogStream(uint8 level);
	ConsoleLogStream(const String& id, uint8 level);
	~ConsoleLogStream();

protected:
	void OutStrEx(const String& str);
	void InitConsole();
};

CLOSE_O2_NAMESPACE