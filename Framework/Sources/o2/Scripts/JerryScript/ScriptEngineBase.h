#if defined(SCRIPTING_BACKEND_JERRYSCRIPT)
#include "jerryscript/jerry-core/include/jerryscript-core.h"

namespace o2
{
	class ScriptParseResultBase
	{
	public:
		virtual ~ScriptParseResultBase();

	protected:
		jerry_value_t mParsedCode;

		friend class ScriptEngine;
	};

	class ScriptEngineBase
	{

	};
}

#endif
