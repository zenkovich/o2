#pragma once

#include "Utils/Containers/Array.h"
#include "Utils/String.h"

namespace o2
{
	/** Basic log stream. Contains interfaces of outing data, parent and child streams. */
	class LogStream
	{
	protected:
		typedef Array<LogStream*> LogSteamsArr;

		LogStream*   mParentStream; /**< Parent stream. NULL if no parent. */
		WString      mId;           /**< Name of log stream. */
		LogSteamsArr mChildStreams; /**< Child streams. */

	public:
		LogStream();
		LogStream(const WString& id);
		virtual ~LogStream();

		/** Return name of stream. */
		const WString& GetId() const;

		/** Binding child stream. */
		void BindStream(LogStream* stream);

		/** Unbinding child stream. Function destroying stream object. */
		void UnbindStream(LogStream* stream, bool release = true);

		/** Unbind and destroy all child streams. */
		void UnbindAllStreams();

		/** Returns parent stream. Null if no parent. */
		LogStream* GetParentStream() const;

		/** Out with low level log. */
		void Out(WString format, ...);

		/** Out error message. */
		void Error(WString format, ...);

		/** Out warning message. */
		void Warning(WString format, ...);

		/** Out string to current stream and parent stream. */
		void OutStr(const WString& str);

		/** Out error to current stream and parent stream. */
		void ErrorStr(const WString& str);

		/** Out warning to current stream and parent stream. */
		void WarningStr(const WString& str);

	protected:
		/** Out string to stream. */
		virtual void OutStrEx(const WString& str) {}

		/** Out error to stream. */
		virtual void OutErrorEx(const WString& str) { OutStrEx("ERROR:" + str); }

		/** Out warning to stream. */
		virtual void OutWarningEx(const WString& str) { OutStrEx("WARNING:" + str); }
	};
}