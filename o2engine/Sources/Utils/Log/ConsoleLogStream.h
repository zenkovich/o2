#pragma once

#include "Utils/Log/LogStream.h"

namespace o2
{
	class ConsoleLogStream: public LogStream
	{
	public:
		ConsoleLogStream();
		ConsoleLogStream(const WString& id);
		~ConsoleLogStream();

	protected:
		void OutStrEx(const WString& str);
		void InitConsole();
	};
}