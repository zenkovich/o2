#pragma once

#include "Utils/Log/LogStream.h"

namespace o2
{
	class ConsoleLogStream: public LogStream
	{
	public:
		ConsoleLogStream();
		ConsoleLogStream(const TString& id);
		~ConsoleLogStream();

	protected:
		void OutStrEx(const TString& str);
		void InitConsole();
	};
}