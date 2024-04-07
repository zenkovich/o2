#include "o2/stdafx.h"
#include "FileLogStream.h"

#include <fstream>

namespace o2
{
    FileLogStream::FileLogStream(const String& fileName):
        LogStream(), filename(fileName)
    {
        std::fstream ofs(filename, std::ios::out);
        if (ofs) 
            ofs.close();
    }

    FileLogStream::FileLogStream(const WString& id, const String& fileName):
        LogStream(id), filename(fileName)
    {
        std::fstream ofs(filename, std::ios::out);
        if (ofs)
            ofs.close();
    }

    void FileLogStream::RefConstruct(const WString& id, const String& fileName)
    {
        this->filename = fileName;
    }

    FileLogStream::~FileLogStream()
    {
    }

    void FileLogStream::OutStrEx(const WString& str)
    {
        /*std::fstream ofs(mFilename, std::ios::app);
        if (ofs)
        {
            ofs << ((String)str).Data() << std::endl;
            ofs.close();
        }*/
    }
}
