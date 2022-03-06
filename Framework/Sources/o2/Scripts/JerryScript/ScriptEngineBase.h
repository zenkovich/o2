#if defined(SCRIPTING_BACKEND_JERRYSCRIPT)
#include "jerryscript/jerry-core/include/jerryscript-core.h"

namespace o2
{
	class LogStream;

	class ScriptParseResultBase
	{
	public:
		ScriptParseResultBase() = default;
		ScriptParseResultBase(const ScriptParseResultBase& other);
		virtual ~ScriptParseResultBase();

	protected:
		jerry_value_t mParsedCode;

		friend class ScriptEngine;
	};

	class ScriptEngineBase
	{
	protected:
		LogStream* mLog = nullptr; // Scripting log stream

	protected:
		static void ErrorCallback(const jerry_value_t error_object, void* user_p);
	};
}

#endif
