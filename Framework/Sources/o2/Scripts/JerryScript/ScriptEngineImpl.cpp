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

		GetGlobal().SetProperty("Dump", Function<String(const ScriptValue&)>([](const ScriptValue& v) { return v.Dump(); }));

		RunBuildtinScripts();
		InitializeBasicPrototypes();
		RegisterTypes();
	}

	ScriptEngine::~ScriptEngine()
	{
		delete ScriptValuePrototypes::GetVec2Prototype();
		delete ScriptValuePrototypes::GetRectPrototype();
		delete ScriptValuePrototypes::GetBorderPrototype();
		delete ScriptValuePrototypes::GetColor4Prototype();
		jerry_cleanup();
	}

	ScriptParseResult ScriptEngine::Parse(const String& script, const String& filename /*= ""*/)
	{
		ScriptParseResult res;
		res.mParsedCode = jerry_parse((jerry_char_t*)filename.Data(), filename.Length(),
			(jerry_char_t*)script.Data(), script.Length(), JERRY_PARSE_MODULE);

		return res;
	}

	ScriptValue ScriptEngine::Run(const ScriptParseResult& parseResult)
	{
		ScriptValue res;
		res.Accept(jerry_run(parseResult.mParsedCode));
		return res;
	}

	ScriptValue ScriptEngine::Eval(const String& script, const String& filename /*= ""*/)
	{
		auto parseRes = Parse(script, filename);
		if (parseRes.IsOk())
			return Run(parseRes);

		return ScriptValue();
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

	void ScriptEngineBase::InitializeBasicPrototypes()
	{
		auto global = o2Scripts.GetGlobal();
		ScriptValuePrototypes::GetVec2Prototype() = mnew ScriptValue(o2Scripts.Eval("Vec2.prototype"));
		ScriptValuePrototypes::GetRectPrototype() = mnew ScriptValue(o2Scripts.Eval("Rect.prototype"));
		ScriptValuePrototypes::GetBorderPrototype() = mnew ScriptValue(o2Scripts.Eval("Border.prototype"));
		ScriptValuePrototypes::GetColor4Prototype() = mnew ScriptValue(o2Scripts.Eval("Color4.prototype"));
	}
}

#endif