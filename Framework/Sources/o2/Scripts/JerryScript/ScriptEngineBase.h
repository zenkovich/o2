#if defined(SCRIPTING_BACKEND_JERRYSCRIPT)
#include "jerryscript/jerry-core/include/jerryscript-core.h"

#include "o2/Utils/Types/Ref.h"

namespace o2
{
	class LogStream;
	FORWARD_REF(LogStream);

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
		Ref<LogStream> mLog; // Scripting log stream

	protected:
		ScriptEngineBase();

		static void ErrorCallback(const jerry_value_t error_object, void* user_p);
		static jerry_value_t PrintCallback(const jerry_value_t func_obj_val, const jerry_value_t this_p,
										   const jerry_value_t args_p[], const jerry_length_t args_cnt);

		// Initialized basic prototypes for math and other
		void InitializeBasicPrototypes();
	};
}

#endif
