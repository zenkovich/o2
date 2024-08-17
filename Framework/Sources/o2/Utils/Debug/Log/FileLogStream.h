#pragma once

#include "o2/Utils/Debug/Log/LogStream.h"

namespace o2
{
    // ----------------------------------------
    // File log stream, puts messages into file
    // ----------------------------------------
    class FileLogStream:public LogStream
    {
    public:
        // Constructor with file name
        FileLogStream(const String& fileName);

        // Constructor with id and file name
        FileLogStream(const WString& id, const String& fileName);

        // Destructor
        ~FileLogStream();

    protected:
        std::ofstream mStream; // Output stream

    protected:
        // Opens file stream
        void OpenStream(const String& fileName);

        // Outs string into file
        void OutStrEx(const WString& str);
    };
}
