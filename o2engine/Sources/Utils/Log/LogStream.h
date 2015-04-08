#pragma once

#include <vector>

namespace o2
{
	/** Basic log stream. Contains interfaces of outing data, parent and child streams. */
	class LogStream
	{
	protected:
		typedef std::vector<LogStream*> LogSteamsVec;

		LogStream*   mParentStream; /**< Parent stream. NULL if no parent. */
		std::string  mId;           /**< Name of log stream. */
		LogSteamsVec mChildStreams; /**< Child streams. */

	public:
		LogStream();
		LogStream(const std::string& id);
		virtual ~LogStream();

		/** Return name of stream. */
		const std::string& GetId() const;

		/** Binding child stream. */
		void BindStream(LogStream* stream);

		/** Unbinding child stream. Function destroying stream object. */
		void UnbindStream(LogStream* stream, bool release = true);

		/** Unbind and destroy all child streams. */
		void UnbindAllStreams();

		/** Returns parent stream. Null if no parent. */
		LogStream* GetParentStream() const;

		/** Out with low level log. */
		void Out(const char* format, ...);

		/** Out error message. */
		void Error(const char* format, ...);

		/** Out warning message. */
		void Warning(const char* format, ...);

	protected:
		/** Out string to stream. */
		virtual void OutStrEx(const std::string& str) {}

		/** Out error to stream. */
		virtual void OutErrorEx(const std::string& srt) { OutStrEx("ERROR:" + srt); }

		/** Out warning to stream. */
		virtual void OutWarningEx(const std::string& srt) { OutStrEx("WARNING:" + srt); }

		/** Out string to current stream and parent stream. */
		void OutStr(const std::string& str);

		/** Out error to current stream and parent stream. */
		void OutError(const std::string& str);

		/** Out warning to current stream and parent stream. */
		void OutWarning(const std::string& str);
	};
}