#include "Debug.h"

#include <cstdarg>
#include "Utils/Log/LogStream.h"
#include "Utils/Log/FileLogStream.h"
#include "Utils/Log/ConsoleLogStream.h"

namespace o2
{
	CREATE_SINGLETON(Debug);

	Debug::Debug()
	{
		FileLogStream* fileLogStream = new FileLogStream("", "log.txt");
		mLogStream = new ConsoleLogStream("");
		fileLogStream->BindStream(mLogStream);
	}

	Debug::~Debug()
	{
		delete mLogStream->GetParentStream();
	}

	void Debug::Log(const String& out, ...)
	{
		va_list vlist;
		va_start(vlist, out);

		mInstance->mLogStream->OutStr(FormatStr(out, vlist));

		va_end(vlist);
	}

	void Debug::LogStr(const String& out)
	{
		mInstance->mLogStream->OutStr(out);
	}

	void Debug::LogWarning(const String& out, ...)
	{
		va_list vlist;
		va_start(vlist, out);

		mInstance->mLogStream->WarningStr(FormatStr(out, vlist));

		va_end(vlist);
	}

	void Debug::LogWarningStr(const String& out)
	{
		mInstance->mLogStream->WarningStr(out);
	}

	void Debug::LogError(const String& out, ...)
	{
		va_list vlist;
		va_start(vlist, out);

		mInstance->mLogStream->ErrorStr(FormatStr(out, vlist));

		va_end(vlist);
	}

	void Debug::LogErrorStr(const String& out)
	{
		mInstance->mLogStream->ErrorStr(out);
	}

}