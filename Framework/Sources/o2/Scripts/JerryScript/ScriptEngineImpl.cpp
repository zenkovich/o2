#include "o2/stdafx.h"

#if defined(SCRIPTING_BACKEND_JERRYSCRIPT)
#include "o2/Scripts/ScriptEngine.h"
#include "jerryscript/jerry-ext/include/jerryscript-ext/handler.h"

namespace o2
{
	ScriptParseResultBase::~ScriptParseResultBase()
	{
		jerry_release_value(mParsedCode);
	}

	bool ScriptParseResult::IsOk() const
	{
		return jerry_value_is_error(mParsedCode);
	}

	String ScriptParseResult::GetError() const
	{
		return "Unknown";
	}

	ScriptEngine::ScriptEngine()
	{
		jerry_init(JERRY_INIT_EMPTY);
		jerryx_handler_register_global((const jerry_char_t*)"print", jerryx_handler_print);

		RegisterTypes();
	}

	ScriptEngine::~ScriptEngine()
	{
		jerry_cleanup();
	}

	ScriptParseResult ScriptEngine::Parse(const String& script)
	{
		ScriptParseResult res;
		res.mParsedCode = jerry_parse(nullptr, 0, (jerry_char_t*)script.Data(), script.Length(), JERRY_PARSE_NO_OPTS);
		return res;
	}

	bool ScriptEngine::Run(const ScriptParseResult& parseResult)
	{
		jerry_value_t retValue = jerry_run(parseResult.mParsedCode);
		bool result = !jerry_value_is_error(retValue);
		jerry_release_value(retValue);

		return result;
	}

	ScriptValue ScriptEngine::Eval(const String& script)
	{
		ScriptValue res;
		res.Accept(jerry_eval((jerry_char_t*)script.Data(), script.Length(), JERRY_PARSE_NO_OPTS));

		if (res.GetValueType() == ScriptValue::ValueType::Error)
			o2Debug.Log("Failed to evaluate: " + res.GetError());

		return res;
	}

	ScriptValue ScriptEngine::GetGlobal() const
	{
		ScriptValue res;
		res.Accept(jerry_get_global_object());
		return res;
	}

	void ScriptEngine::CollectGarbage() const
	{
		jerry_gc(JERRY_GC_PRESSURE_HIGH);
	}

}

#endif