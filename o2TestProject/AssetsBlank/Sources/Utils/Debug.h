#pragma once

#include "String.h"
#include "Utils/Memory/Ptr.h"
#include "Utils/Singleton.h"

namespace o2
{
	class LogStream;

// Debug stuff access macros
#define Debug DebugStuff::Instance()

	// ---------------
	// Debugging stuff
	// ---------------
	class DebugStuff: public Singleton<DebugStuff>
	{
		friend class Singleton<DebugStuff>;

		Ptr<LogStream> mLogStream; // Main log stream

	private:
		// Default constructor
		DebugStuff();

		// Destructor
		~DebugStuff();

		// Protect copying
		DebugStuff operator=(const DebugStuff& other);

	public:
		// Out message into main log with formatting
		void Log(WString format, ...);

		// Out message string into main log
		void LogStr(const WString& out);

		// Out warning message into main log with formatting
		void LogWarning(WString format, ...);

		// Out warning message string into main log
		void LogWarningStr(const WString& out);

		// Out error message into main log with formatting
		void LogError(WString format, ...);

		// Out error message string into main log
		void LogErrorStr(const WString& out);

		// Returns pointer to main log
		Ptr<LogStream> GetLog();
	};
}