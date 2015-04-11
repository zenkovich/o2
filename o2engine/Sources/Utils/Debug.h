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
		static void Log(const String& out, ...);
		static void LogStr(const String& out);
		static void LogWarning(const String& out, ...);
		static void LogWarningStr(const String& out);
		static void LogError(const String& out, ...);
		static void LogErrorStr(const String& out);
	};
}