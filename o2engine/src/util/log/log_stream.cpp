#include "log_stream.h"

#include <algorithm>
#include <cstdarg>

OPEN_O2_NAMESPACE

const char* cLogStream::BindValue::getStr()
{
	switch (mType)
	{
	case BV_INT:
		sprintf(mBuffer, "%.4i", *((int*)mValuePtr));
		break;
		
	case BV_BOOL:
		if ( *((bool*)mValuePtr) )
			strcpy(mBuffer, "true");
		else
			strcpy(mBuffer, "false");
		break;
		
	case BV_FLOAT:
		sprintf(mBuffer, "%.4f", *((float*)mValuePtr));
		break;

	case BV_CHAR_PTR:
		strncpy(mBuffer, (char*)mValuePtr, 255);
		mBuffer[255] = '\0';
		break;
		
	case BV_STRING:
		strncpy(mBuffer, (*(std::string*)mValuePtr).c_str(), 255);
		mBuffer[255] = '\0';
		break;
	};

	return mBuffer;
}

cLogStream::cLogStream():
	mParentStream(NULL), mLevel(2)
{

}

cLogStream::cLogStream( const std::string& id ):
	mParentStream(NULL), mId(id), mLevel(2)
{
}

cLogStream::~cLogStream()
{
	unbindAllStreams();
	unbindAllValues();
}

void cLogStream::setLevel( uint8 level )
{
	mLevel = level;

	for (LogSteamsVec::iterator it = mChildStreams.begin(); it != mChildStreams.end(); ++it)
		(*it)->setLevel(level);
}

uint8 cLogStream::getLevel() const
{
	return mLevel;
}

const std::string& cLogStream::getId() const
{
	return mId;
}

void cLogStream::bindStream( cLogStream* stream )
{
	stream->mParentStream = this;
	stream->mLevel = mLevel;
	mChildStreams.push_back(stream);
}

void cLogStream::unbindStream( cLogStream* stream )
{
	LogSteamsVec::iterator fnd = std::find(mChildStreams.begin(), mChildStreams.end(), stream);
	if (fnd != mChildStreams.end())
		mChildStreams.erase(fnd);

	safe_release(stream);
}

void cLogStream::unbindAllStreams()
{
	for (LogSteamsVec::iterator it = mChildStreams.begin(); it != mChildStreams.end(); ++it)
	{
		safe_release(*it);
	}

	mChildStreams.clear();
}

void cLogStream::bindValue( void* valuePtr, BindValType type, const std::string& id )
{
	mBindedValues.push_back(BindValue(valuePtr, type, id));
}

void cLogStream::unbindvalue( void* valuePtr )
{
	for (BindValVec::iterator it = mBindedValues.begin(); it != mBindedValues.end(); ++it)
	{
		if (valuePtr == it->mValuePtr)
		{
			mBindedValues.erase(it);
			break;
		}
	}
}

void cLogStream::unbindAllValues()
{
	mBindedValues.clear();
}

void cLogStream::checkBindedValues()
{
	for (BindValVec::iterator it = mBindedValues.begin(); it != mBindedValues.end(); ++it)
	{
		outStrEx(it->getStr());
	}

	for (LogSteamsVec::iterator it = mChildStreams.begin(); it != mChildStreams.end(); ++it)
	{
		(*it)->checkBindedValues();
	}
}

void cLogStream::out( const char* format, ... )
{
	if (mLevel > 0)
	{
		va_list vlist;
		va_start(vlist, format);

		char buf[1024]; 
		vsprintf(buf, format, vlist);

		va_end(vlist);

		outStr(buf);
	}
}

void cLogStream::hout( const char* format, ... )
{
	if (mLevel > 1)
	{
		va_list vlist;
		va_start(vlist, format);

		char buf[1024]; 
		vsprintf(buf, format, vlist);

		va_end(vlist);

		outStr(buf);
	}
}

cLogStream* cLogStream::getParentStream() const
{
	return mParentStream;
}

void cLogStream::outStr( const std::string& str )
{
	outStrEx(str);

	if (mParentStream)
	{
		if (mId == "")
			mParentStream->outStr(str);
		else			
			mParentStream->outStr(mId + ":" + str);
	}
}

CLOSE_O2_NAMESPACE