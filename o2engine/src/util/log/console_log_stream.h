#ifndef CONSOLE_LOG_STREAM_H
#define CONSOLE_LOG_STREAM_H

#include "public.h"
#include "log_stream.h"

OPEN_O2_NAMESPACE

class cConsoleLogStream:public cLogStream
{
public:
	cConsoleLogStream(uint8 level);
	cConsoleLogStream(const std::string& id, uint8 level);
	~cConsoleLogStream();

protected:
	void outStrEx(const std::string& str);
	void initConsole();
};

CLOSE_O2_NAMESPACE

#endif //CONSOLE_LOG_STREAM_H