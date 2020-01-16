#include "o2/stdafx.h"
#include "LogStream.h"

#include <cstdarg>

namespace o2
{
	LogStream::LogStream():
		mParentStream(nullptr)
	{}

	LogStream::LogStream(const WString& id) :
		mParentStream(nullptr), mId(id)
	{}

	LogStream::~LogStream()
	{
		if (mParentStream)
			mParentStream->UnbindStream(this);

		UnbindAllStreams();
	}

	const WString& LogStream::GetId() const
	{
		return mId;
	}

	void LogStream::BindStream(LogStream* stream)
	{
		stream->mParentStream = this;
		mChildStreams.Add(stream);
	}

	void LogStream::UnbindStream(LogStream* stream)
	{
		mChildStreams.Remove(stream);
	}

	void LogStream::UnbindAndReleaseStream(LogStream* stream)
	{
		mChildStreams.Remove(stream);
		delete stream;
	}

	void LogStream::UnbindAllStreams()
	{
		auto streams = mChildStreams;
		for (auto stream : streams)
			delete stream;

		mChildStreams.Clear();
	}

	void LogStream::Out(WString format, ...)
	{
		va_list vlist;
		va_start(vlist, format);

		OutStr(WString::Format(format, vlist));

		va_end(vlist);
	}

	void LogStream::Error(WString format, ...)
	{
		va_list vlist;
		va_start(vlist, format);

		ErrorStr(WString::Format(format, vlist));

		va_end(vlist);
	}

	void LogStream::Warning(WString format, ...)
	{
		va_list vlist;
		va_start(vlist, format);

		WarningStr(WString::Format(format, vlist));

		va_end(vlist);
	}

	LogStream* LogStream::GetParentStream() const
	{
		return mParentStream;
	}

	void LogStream::OutStr(const WString& str)
	{
		OutStrEx(str);

		if (mParentStream)
		{
			if (mId.IsEmpty())
				mParentStream->OutStr(str);
			else
				mParentStream->OutStr((mId + ":" + str));
		}
	}

	void LogStream::ErrorStr(const WString& str)
	{
		OutErrorEx(str);

		if (mParentStream)
		{
			if (mId == "")
				mParentStream->ErrorStr(str);
			else
				mParentStream->ErrorStr((mId + ":" + str));
		}
	}

	void LogStream::WarningStr(const WString& str)
	{
		OutWarningEx(str);

		if (mParentStream)
		{
			if (mId == "")
				mParentStream->WarningStr(str);
			else
				mParentStream->WarningStr((mId + ":" + str));
		}
	}

	void LogStream::OutErrorEx(const WString& str)
	{
		OutStrEx("ERROR:" + str);

	if (IsStoppingOnLogErrors())
		Assert(false, (const char*)((String)str));
	}

	void LogStream::OutWarningEx(const WString& str)
	{
		OutStrEx("WARNING:" + str);
	}

}
