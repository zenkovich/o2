#include "Utils/Log/LogStream.h"

#include <cstdarg>

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
		mChildStreams.Add(stream);
	}

	void LogStream::UnbindStream(LogStream* stream, bool release /*= true*/)
	{
		mChildStreams.Remove(stream);

		if (release)
			delete stream;
	}

	void LogStream::UnbindAllStreams()
	{
		for (auto stream : mChildStreams)
			delete stream;

		mChildStreams.Clear();
	}

	void LogStream::Out(const char* format, ...)
	{
		va_list vlist;
		va_start(vlist, format);

		char buf[1024];
		vsprintf(buf, format, vlist);

		va_end(vlist);

		OutStr(buf);
	}

	void LogStream::Error(const char* format, ...)
	{
		va_list vlist;
		va_start(vlist, format);

		char buf[1024];
		vsprintf(buf, format, vlist);

		va_end(vlist);

		ErrorStr(buf);
	}

	void LogStream::Warning(const char* format, ...)
	{
		va_list vlist;
		va_start(vlist, format);

		char buf[1024];
		vsprintf(buf, format, vlist);

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
				mParentStream->Out(str.c_str());
			else
				mParentStream->Out((mId + ":" + str).c_str());
		}
	}

	void LogStream::ErrorStr(const String& str)
	{
		OutErrorEx(str);

		if (mParentStream)
		{
			if (mId == "")
				mParentStream->Error(str.c_str());
			else
				mParentStream->Error((mId + ":" + str).c_str());
		}
	}

	void LogStream::WarningStr(const String& str)
	{
		OutWarningEx(str);

		if (mParentStream)
		{
			if (mId == "")
				mParentStream->Warning(str.c_str());
			else
				mParentStream->Warning((mId + ":" + str).c_str());
		}
	}
}