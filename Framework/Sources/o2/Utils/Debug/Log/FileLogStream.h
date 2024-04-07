#pragma once

#include "o2/Utils/Debug/Log/LogStream.h"

namespace o2
{
    // ----------------------------------------
    // File log stream, puts messages into file
    // ----------------------------------------
    class FileLogStream:public LogStream
    {
        String filename; // Target file

    public:
        // Constructor with file name
        FileLogStream(const String& fileName);

        // Constructor with id and file name
        FileLogStream(const WString& id, const String& fileName);

        void RefConstruct(const WString& id, const String& fileName);

        // Destructor
        ~FileLogStream();

    protected:
        // Outs string into file
        void OutStrEx(const WString& str);
    };
}
