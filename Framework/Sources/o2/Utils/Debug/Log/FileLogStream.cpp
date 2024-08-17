#include "o2/stdafx.h"
#include "FileLogStream.h"

#include <fstream>

namespace o2
{
    FileLogStream::FileLogStream(const String& fileName):
        LogStream()
    {
        OpenStream(fileName);
    }

    FileLogStream::FileLogStream(const WString& id, const String& fileName):
        LogStream(id)
    {
        OpenStream(fileName);
    }

    FileLogStream::~FileLogStream()
    {
        if (mStream)
            mStream.close();
    }

    void FileLogStream::OpenStream(const String& fileName)
    {
        mStream.open(fileName, std::ios::out);
        Assert(mStream, "Can't open file for logging");
    }

    void FileLogStream::OutStrEx(const WString& str)
    {
        if (mStream)
            mStream << (String)str << std::endl;
    }
}
