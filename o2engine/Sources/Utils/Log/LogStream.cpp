#include "Utils/Log/LogStream.h"

#include <cstdarg>
#include <algorithm>

namespace o2
{
	LogStream::LogStream():
		mParentStream(NULL)
	{
	}

	LogStream::LogStream(const std::string& id):
		mParentStream(NULL), mId(id)
	{
	}

	LogStream::~LogStream()
	{
		if (mParentStream)
			mParentStream->UnbindStream(this, false);

		UnbindAllStreams();
	}

	const std::string& LogStream::GetId() const
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

		OutError(buf);
	}

	void LogStream::Warning(const char* format, ...)
	{
		va_list vlist;
		va_start(vlist, format);

		char buf[1024];
		vsprintf(buf, format, vlist);

		va_end(vlist);

		OutWarning(buf);
	}

	LogStream* LogStream::GetParentStream() const
	{
		return mParentStream;
	}

	void LogStream::OutStr(const std::string& str)
	{
		OutStrEx(str);

		if (mParentStream)
		{
			if (mId == "")
				mParentStream->OutStr(str);
			else
				mParentStream->OutStr(mId + ":" + str);
		}
	}

	void LogStream::OutError(const std::string& str)
	{
		OutErrorEx(str);

		if (mParentStream)
		{
			if (mId == "")
				mParentStream->OutError(str);
			else
				mParentStream->OutError(mId + ":" + str);
		}
	}

	void LogStream::OutWarning(const std::string& str)
	{
		OutWarningEx(str);

		if (mParentStream)
		{
			if (mId == "")
				mParentStream->OutWarning(str);
			else
				mParentStream->OutWarning(mId + ":" + str);
		}
	}
}