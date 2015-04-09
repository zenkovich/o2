#pragma once

#include "Utils/Log/LogStream.h"

namespace o2
{
	class FileLogStream:public LogStream
	{
		std::string mFilename;

	public:
		FileLogStream(const std::string& fileName);
		FileLogStream(const std::string& id, const std::string& fileName);
		~FileLogStream();

	protected:
		void OutStrEx(const std::string& str);
	};
}