#pragma once

#include "String.h"
#include "Utils/Memory/Ptr.h"
#include "Utils/Singleton.h"

namespace o2
{
	class LogStream;

	class Debug : public Singleton<Debug>
	{
		friend class Singleton<Debug>;

		Ptr<LogStream> mLogStream; /** Main log stream. */

	private:
		/** ctor. */
		Debug();

		/** dtor. */
		~Debug();

		/** Protect copying. */
		Debug operator=(const Debug& other);

	public:
		/** Out message into main log with formatting. */
		static void Log(WString format, ...);

		/** Out message string into main log. */
		static void Log(const WString& out);

		/** Out warning message into main log with formatting. */
		static void LogWarning(WString format, ...);

		/** Out warning message string into main log. */
		static void LogWarning(const WString& out);

		/** Out error message into main log with formatting. */
		static void LogError(WString format, ...);

		/** Out error message string into main log. */
		static void LogError(const WString& out);

		/** Returns pointer to main log. */
		static Ptr<LogStream> GetLog();
	};
}