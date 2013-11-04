#ifndef FILE_LOG_STREAM_H
#define FILE_LOG_STREAM_H

#include "public.h"
#include "log_stream.h"

OPEN_O2_NAMESPACE

class cOutFile;

class cFileLogStream:public cLogStream
{
	cOutFile* mOutFile;

public:
	cFileLogStream(uint8 level, const std::string& fileName);
	cFileLogStream(const std::string& id, uint8 level, const std::string& fileName);
	~cFileLogStream();

protected:
	void outStrEx(const std::string& str);
	void openFile(const std::string& fileName);
};

CLOSE_O2_NAMESPACE

#endif //FILE_LOG_STREAM_H