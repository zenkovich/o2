#pragma once

#include "log_stream.h"

OPEN_O2_NAMESPACE

class OutFile;

class FileLogStream:public LogStream
{
	OutFile* mOutFile;

public:
	FileLogStream(uint8 level, const String& fileName);
	FileLogStream(const String& id, uint8 level, const String& fileName);
	~FileLogStream();

protected:
	void OutStrEx(const String& str);
	void OutErrorEx(const String& srt);
	void OutWarningEx(const String& srt);
	void OpenFile(const String& fileName);
};

CLOSE_O2_NAMESPACE