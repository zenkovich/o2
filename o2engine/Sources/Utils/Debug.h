#pragma once

#include "String.h"
#include "Utils/Memory/Ptr.h"
#include "Utils/Singleton.h"

// Debug stuff access macros
#define o2Debug Debug::Instance()

namespace o2
{
	class LogStream;

	// ---------------
	// Debugging stuff
	// ---------------
	class Debug: public Singleton<Debug>
	{
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

	protected:
		Ptr<LogStream> mLogStream; // Main log stream

	private:
		// Default constructor
		Debug();

		// Destructor
		~Debug();

		// Protect copying
		Debug operator=(const Debug& other);

		friend class Singleton<Debug>;
	};
}