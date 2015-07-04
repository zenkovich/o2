#include "Utils/Log/LogStream.h"

#include <cstdarg>

namespace o2
{
	LogStream::LogStream():
		mParentStream(NULL)
	{
	}

	LogStream::LogStream(const WString& id):
		mParentStream(NULL), mId(id)
	{
	}

	LogStream::~LogStream()
	{
		if (mParentStream)
			mParentStream->UnbindStream(this, false);

		UnbindAllStreams();
	}

	const WString& LogStream::GetId() const
	{
		return mId;
	}

	void LogStream::BindStream(const Ptr<LogStream>& stream)
	{
		stream->mParentStream = this;
		mChildStreams.Add(stream);
	}

	void LogStream::UnbindStream(const Ptr<LogStream>& stream, bool release /*= true*/)
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

	Ptr<LogStream> LogStream::GetParentStream() const
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
}