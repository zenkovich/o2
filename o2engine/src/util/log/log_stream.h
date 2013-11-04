#ifndef LOG_STREAM_H
#define LOG_STREAM_H

#include <vector>
#include <list>

#include "public.h"
OPEN_O2_NAMESPACE

/** Basic log stream. Contains interfaces of outing data, binding values, parent and child streams. */
class cLogStream
{
protected:
	/** Type of bind value. */
	enum BindValType { BV_INT = 0, BV_FLOAT, BV_BOOL, BV_CHAR_PTR, BV_STRING, BV_VEC2F };

	/** Binded value struct. Contains pointer to binded value, type, id. */
	struct BindValue
	{
		void*       mValuePtr;    /**< Pointer to binded value. */
		BindValType mType;        /**< Type of binded value. */
		char        mBuffer[256]; /**< Buffer for formatted data by type. */
		std::string mId;          /**< Id of binded value. Using as prefix .*/

		BindValue(void* valuePtr, BindValType type, const std::string& id):
			mValuePtr(valuePtr), mType(type), mId(id) { mBuffer[0] = '\0'; }

		/** Returns formatted string data by value type. */
		const char* getStr();
	};
	typedef std::vector<BindValue> BindValVec;
	typedef std::vector<cLogStream*> LogSteamsVec;

	cLogStream*  mParentStream; /**< Parent stream. NULL if no parent. */

	std::string  mId;           /**< Name of log stream. */
	uint8        mLevel;        /**< Log level. */

	LogSteamsVec mChildStreams; /**< Child streams. */
	BindValVec   mBindedValues; /**< Binded values. */

public:
	cLogStream();
	cLogStream(const std::string& id);
	virtual ~cLogStream();

	/** Sets log level, for childs too. */
	void setLevel(uint8 level);

	/** Returns log level. */
	uint8 getLevel() const;

	/** Return name of stream. */
	const std::string& getId() const;

	/** Binding child stream. */
	void bindStream(cLogStream* stream);

	/** Unbinding child stream. Function destroying stream object. */
	void unbindStream(cLogStream* stream);

	/** Unbind and destroy all child streams. */
	void unbindAllStreams();

	/** Returns parent stream. Null if no parent. */
	cLogStream* getParentStream() const;

	/** Binding value. */
	void bindValue(void* valuePtr, BindValType type, const std::string& id);

	/** Unbind value. */
	void unbindvalue(void* valuePtr);

	/** Unbind all values. */
	void unbindAllValues();

	/** Out all binded values data. */
	void checkBindedValues();

	/** Out with low level log. */
	void out(const char* format, ...);

	/** Out with hight level log. */
	void hout(const char* format, ...);

protected:
	/** Out string to stream. */
	virtual void outStrEx(const std::string& str) {}

	/** Out string to current stream and parant stream. */
	void outStr(const std::string& str);
};

CLOSE_O2_NAMESPACE

#endif //LOG_STREAM_H