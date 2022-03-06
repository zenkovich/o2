#include "o2/stdafx.h"

#if defined(SCRIPTING_BACKEND_JERRYSCRIPT)
#include "jerryscript/jerry-ext/include/jerryscript-ext/handler.h"
#include "o2/Scripts/ScriptEngine.h"
#include "o2/Utils/Debug/Log/LogStream.h"

namespace o2
{
	void ScriptEngineBase::ErrorCallback(const jerry_value_t error_object, void* user_p)
	{
		auto strValue = jerry_value_to_string(error_object);
		ScriptValue tmp;
		tmp.AcquireValue(strValue);
		o2Scripts.mLog->ErrorStr(tmp.GetValue<String>());
	}

	ScriptParseResultBase::~ScriptParseResultBase()
	{
		jerry_release_value(mParsedCode);
	}

	ScriptParseResultBase::ScriptParseResultBase(const ScriptParseResultBase& other)
	{
		mParsedCode = jerry_acquire_value(other.mParsedCode);
	}

	bool ScriptParseResult::IsOk() const
	{
		return !jerry_value_is_error(mParsedCode);
	}

	String ScriptParseResult::GetError() const
	{
		ScriptValue tmp;
		tmp.AcquireValue(mParsedCode);
		return tmp.GetError();
	}

	ScriptEngine::ScriptEngine()
	{
		mLog = mnew LogStream("Scripting");
		o2Debug.GetLog()->BindStream(mLog);

		jerry_init(JERRY_INIT_EMPTY);
		jerryx_handler_register_global((const jerry_char_t*)"print", jerryx_handler_print);
		jerry_set_error_object_created_callback(&ErrorCallback, NULL);

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