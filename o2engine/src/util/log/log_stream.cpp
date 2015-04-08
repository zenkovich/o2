#include "log_stream.h"

#include <cstdarg>
#include <algorithm>
#include "util/mem_utils/alloc_operators.h"

OPEN_O2_NAMESPACE


LogStream::LogStream():
mParentStream(NULL), mLevel(2)
{
}

LogStream::LogStream(const String& id):
mParentStream(NULL), mId(id), mLevel(2)
{
}

LogStream::~LogStream()
{
	if (mParentStream)
		mParentStream->UnbindStream(this, false);
	UnbindAllStreams();
}

void LogStream::SetLevel(uint8 level)
{
	mLevel = level;

	for (auto it : mChildStreams)
		it->SetLevel(level);
}

uint8 LogStream::GetLevel() const
{
	return mLevel;
}

const String& LogStream::GetId() const
{
	return mId;
}

void LogStream::BindStream(LogStream* stream)
{
	stream->mParentStream = this;
	stream->mLevel = mLevel;
	mChildStreams.push_back(stream);
}

void LogStream::UnbindStream(LogStream* stream, bool release /*= true*/)
{
	auto fnd = std::find(mChildStreams.begin(), mChildStreams.end(), stream);
	if (fnd != mChildStreams.end())
		mChildStreams.erase(fnd);

	if (release)
		SafeRelease(stream);
}

void LogStream::UnbindAllStreams()
{
	//ReleaseArray(mChildStreams);
}

void LogStream::Out(const char* format, ...)
{
	if (mLevel > 0)
	{
		va_list vlist;
		va_start(vlist, format);

		char buf[1024];
		vsprintf(buf, format, vlist);

		va_end(vlist);

		OutStr(buf);
	}
}

void LogStream::HOut(const char* format, ...)
{
	if (mLevel > 1)
	{
		va_list vlist;
		va_start(vlist, format);

		char buf[1024];
		vsprintf(buf, format, vlist);

		va_end(vlist);

		OutStr(buf);
	}
}

void LogStream::Error(const char* format, ...)
{
	if (mLevel > 0)
	{
		va_list vlist;
		va_start(vlist, format);

		char buf[1024];
		vsprintf(buf, format, vlist);

		va_end(vlist);

		OutError(buf);
	}
}

void LogStream::Warning(const char* format, ...)
{
	if (mLevel > 0)
	{
		va_list vlist;
		va_start(vlist, format);

		char buf[1024];
		vsprintf(buf, format, vlist);

		va_end(vlist);

		OutWarning(buf);
	}
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
			mParentStream->OutStr(str);
		else
			mParentStream->OutStr(mId + ":" + str);
	}
}

void LogStream::OutError(const String& str)
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

void LogStream::OutWarning(const String& str)
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

CLOSE_O2_NAMESPACE