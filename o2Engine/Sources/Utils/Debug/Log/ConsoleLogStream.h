#pragma once

#include "Utils/Debug/Log/LogStream.h"

namespace o2
{
	// ----------------------------------------------
	// Console log stream, puts messages into console
	// ----------------------------------------------
	class ConsoleLogStream: public LogStream
	{
	public:
		// Default constructor, initializing console
		ConsoleLogStream();

		// Constructor with id, initializing console
		ConsoleLogStream(const WString& id);

		// Destructor, deinitializing console
		~ConsoleLogStream();

	protected:
		// Outs string info console
		void OutStrEx(const WString& str);

		// Initializing console
		void InitConsole();
	};
}
