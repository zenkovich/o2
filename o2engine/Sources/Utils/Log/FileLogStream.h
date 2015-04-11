#pragma once

#include "Utils/Log/LogStream.h"

namespace o2
{
	class FileLogStream:public LogStream
	{
		std::string mFilename;

	public:
		FileLogStream(const String& fileName);
		FileLogStream(const String& id, const String& fileName);
		~FileLogStream();

	protected:
		void OutStrEx(const String& str);
	};
}