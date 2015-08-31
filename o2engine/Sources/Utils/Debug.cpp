#include "Debug.h"

#include <cstdarg>
#include "Utils/Log/LogStream.h"
#include "Utils/Log/FileLogStream.h"
#include "Utils/Log/ConsoleLogStream.h"

namespace o2
{
	DebugStuff::DebugStuff()
	{
		Ptr<FileLogStream> fileLogStream = mnew FileLogStream("", "log.txt");
		mLogStream = mnew ConsoleLogStream("");
		fileLogStream->BindStream(mLogStream);
	}

	DebugStuff::~DebugStuff()
	{
		mLogStream->GetParentStream().Release();
	}

	void DebugStuff::Log(WString format, ...)
	{
		va_list vlist;
		va_start(vlist, format);

		mInstance->mLogStream->OutStr(String::Format(format, vlist));

		va_end(vlist);
	}

	void DebugStuff::LogStr(const WString& out)
	{
		mInstance->mLogStream->OutStr(out);
	}

	void DebugStuff::LogWarning(WString format, ...)
	{
		va_list vlist;
		va_start(vlist, format);

		mInstance->mLogStream->WarningStr(String::Format(format, vlist));

		va_end(vlist);
	}

	void DebugStuff::LogWarningStr(const WString& out)
	{
		mInstance->mLogStream->WarningStr(out);
	}

	void DebugStuff::LogError(WString format, ...)
	{
		va_list vlist;
		va_start(vlist, format);

		mInstance->mLogStream->ErrorStr(String::Format(format, vlist));

		va_end(vlist);
	}

	void DebugStuff::LogErrorStr(const WString& out)
	{
		mInstance->mLogStream->ErrorStr(out);
	}

	Ptr<LogStream> DebugStuff::GetLog()
	{
		return mInstance->mLogStream;
	}
}