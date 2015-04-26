#pragma once

#include "Utils/Log/LogStream.h"

namespace o2
{
	class ConsoleLogStream: public LogStream
	{
	public:
		ConsoleLogStream();
		ConsoleLogStream(const String& id);
		~ConsoleLogStream();

	protected:
		void OutStrEx(const String& str);
		void InitConsole();
	};
}