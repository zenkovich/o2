#pragma once

#include "Utils/Log/LogStream.h"

namespace o2
{
	class ConsoleLogStream:public LogStream
	{
	public:
		ConsoleLogStream();
		ConsoleLogStream(const std::string& id);
		~ConsoleLogStream();

	protected:
		void OutStrEx(const std::string& str);
		void InitConsole();
	};
}