#include "Debug.h"

#include <cstdarg>
#include "Utils/Log/LogStream.h"
#include "Utils/Log/FileLogStream.h"
#include "Utils/Log/ConsoleLogStream.h"

namespace o2
{
	Debug::Debug()
	{
		Ptr<FileLogStream> fileLogStream = mnew FileLogStream("", "log.txt");
		mLogStream = mnew ConsoleLogStream("");
		fileLogStream->BindStream(mLogStream);
	}

	Debug::~Debug()
	{
		mLogStream->GetParentStream().Release();
	}

	void Debug::Log(WString format, ...)
	{
		va_list vlist;
		va_start(vlist, format);

		mInstance->mLogStream->OutStr(String::Format(format, vlist));

		va_end(vlist);
	}

	void Debug::LogStr(const WString& out)
	{
		mInstance->mLogStream->OutStr(out);
	}

	void Debug::LogWarning(WString format, ...)
	{
		va_list vlist;
		va_start(vlist, format);

		mInstance->mLogStream->WarningStr(String::Format(format, vlist));

		va_end(vlist);
	}

	void Debug::LogWarningStr(const WString& out)
	{
		mInstance->mLogStream->WarningStr(out);
	}

	void Debug::LogError(WString format, ...)
	{
		va_list vlist;
		va_start(vlist, format);

		mInstance->mLogStream->ErrorStr(String::Format(format, vlist));

		va_end(vlist);
	}

	void Debug::LogErrorStr(const WString& out)
	{
		mInstance->mLogStream->ErrorStr(out);
	}

	Ptr<LogStream> Debug::GetLog()
	{
		return mInstance->mLogStream;
	}
}