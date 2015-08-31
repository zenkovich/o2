#include "Utils/Log/LogStream.h"

#include <cstdarg>

namespace o2
{
	LogStream::LogStream():
		mParentStream(nullptr)
	{
	}

	LogStream::LogStream(const WString& id):
		mParentStream(nullptr), mId(id)
	{
	}

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

	void LogStream::BindStream(const Ptr<LogStream>& stream)
	{
		stream->mParentStream = this;
		mChildStreams.Add(stream);
	}

	void LogStream::UnbindStream(const Ptr<LogStream>& stream)
	{
		mChildStreams.Remove(stream);
	}

	void LogStream::UnbindAndReleaseStream(Ptr<LogStream>& stream)
	{
		mChildStreams.Remove(stream);
		stream.Release();
	}

	void LogStream::UnbindAllStreams()
	{
		for (auto stream : mChildStreams)
			stream.Release();

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

	void LogStream::OutErrorEx(const WString& str)
	{
		OutStrEx("ERROR:" + str);

#if ENABLE_STOPPING_ON_LOG_ERRORS
		Assert(false, (char*)((String)str));
#endif
	}

	void LogStream::OutWarningEx(const WString& str)
	{
		OutStrEx("WARNING:" + str);
	}

}