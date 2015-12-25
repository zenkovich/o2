#include "file_log_stream.h"

#include "util/file_system/file.h"

OPEN_O2_NAMESPACE

FileLogStream::FileLogStream(uint8 level, const String& fileName):
LogStream(), mOutFile(NULL)
{
	SetLevel(level);

	if (level > 0)
		OpenFile(fileName);
}

FileLogStream::FileLogStream(const String& id, uint8 level, const String& fileName):
LogStream(id), mOutFile(NULL)
{
	SetLevel(level);

	if (level > 0)
		OpenFile(fileName);
}

FileLogStream::~FileLogStream()
{
	SafeRelease(mOutFile);
}

void FileLogStream::OutStrEx(const String& str)
{
	if (mOutFile)
		mOutFile->WriteData((void*)(str + '\n').c_str(), str.length() + 1);
}

void FileLogStream::OpenFile(const String& fileName)
{
	mOutFile = mnew OutFile(fileName);
}

void FileLogStream::OutErrorEx(const String& srt)
{
	if (mOutFile)
		mOutFile->WriteData((void*)("ERROR:" + srt + '\n').c_str(), srt.length() + 7);
}

void FileLogStream::OutWarningEx(const String& srt)
{
	if (mOutFile)
		mOutFile->WriteData((void*)("WARNING:" + srt + '\n').c_str(), srt.length() + 9);
}

CLOSE_O2_NAMESPACE