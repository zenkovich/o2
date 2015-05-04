#pragma once

#include "String.h"
#include "Utils/Singleton.h"

namespace o2
{
	class LogStream;

	class Debug: public Singleton<Debug>
	{
		friend class Singleton<Debug>;

		LogStream* mLogStream;

		Debug();
		~Debug();

		Debug operator=(const Debug& other);

	public:
		static void Log(WString format, ...);
		static void Log(const WString& out);
		static void LogWarning(WString format, ...);
		static void LogWarning(const WString& out);
		static void LogError(WString format, ...);
		static void LogError(const WString& out);

		static LogStream* GetLog();
	};
}