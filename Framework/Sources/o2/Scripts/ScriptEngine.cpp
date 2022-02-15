#include "o2/stdafx.h"

#if IS_SCRIPTING_SUPPORTED
#include "ScriptEngine.h"

namespace o2
{
	DECLARE_SINGLETON(ScriptEngine);

	ScriptParseResult::operator bool() const
	{
		return IsOk();
	}

	void ScriptEngine::RegisterTypes()
	{
		auto& types = o2Reflection.GetTypes();
		for (auto kv : types)
		{

		}
	}

}

#endif // IS_SCRIPTING_SUPPORTED