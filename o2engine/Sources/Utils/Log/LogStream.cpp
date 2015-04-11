#include "Utils/Log/LogStream.h"

#include <cstdarg>
#include <algorithm>

namespace o2
{
	LogStream::LogStream():
		mParentStream(NULL)
	{
	}

	LogStream::LogStream(const String& id):
		mParentStream(NULL), mId(id)
	{
	}

	LogStream::~LogStream()
	{
		if (mParentStream)
			mParentStream->UnbindStream(this, false);

		UnbindAllStreams();
	}

	const String& LogStream::GetId() const
	{
		return mId;
	}

	void LogStream::BindStream(LogStream* stream)
	{
		stream->mParentStream = this;
		mChildStreams.push_back(stream);
	}

	void LogStream::UnbindStream(LogStream* stream, bool release /*= true*/)
	{
		auto fnd = std::find(mChildStreams.begin(), mChildStreams.end(), stream);
		if (fnd != mChildStreams.end())
			mChildStreams.erase(fnd);

		if (release)
			delete stream;
	}

	void LogStream::UnbindAllStreams()
	{
		for (auto stream : mChildStreams)
			delete stream;

		mChildStreams.clear();
	}

	void LogStream::Out(const String& format, ...)
	{
		va_list vlist;
		va_start(vlist, format);

		char buf[1024];
		vsprintf(buf, format.c_str(), vlist);

		va_end(vlist);

		OutStr(buf);
	}

	void LogStream::Error(const String& format, ...)
	{
		va_list vlist;
		va_start(vlist, format);

		char buf[1024];
		vsprintf(buf, format.c_str(), vlist);

		va_end(vlist);

		ErrorStr(buf);
	}

	void LogStream::Warning(const String& format, ...)
	{
		va_list vlist;
		va_start(vlist, format);

		char buf[1024];
		vsprintf(buf, format.c_str(), vlist);

		va_end(vlist);

		WarningStr(buf);
	}

	LogStream* LogStream::GetParentStream() const
	{
		return mParentStream;
	}

	void LogStream::OutStr(const String& str)
	{
		OutStrEx(str);

		if (mParentStream)
		{
			if (mId == "")
				mParentStream->Out(str);
			else
				mParentStream->Out(mId + ":" + str);
		}
	}

	void LogStream::ErrorStr(const String& str)
	{
		OutErrorEx(str);

		if (mParentStream)
		{
			if (mId == "")
				mParentStream->Error(str);
			else
				mParentStream->Error(mId + ":" + str);
		}
	}

	void LogStream::WarningStr(const String& str)
	{
		OutWarningEx(str);

		if (mParentStream)
		{
			if (mId == "")
				mParentStream->Warning(str);
			else
				mParentStream->Warning(mId + ":" + str);
		}
	}
}