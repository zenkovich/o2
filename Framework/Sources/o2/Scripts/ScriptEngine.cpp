#include "o2/stdafx.h"

#if IS_SCRIPTING_SUPPORTED
#include "ScriptEngine.h"
#include "o2/Utils/Debug/Log/LogStream.h"
#include "o2/Utils/FileSystem/FileSystem.h"
#include "o2/Utils/System/Time/Timer.h"

namespace o2
{
	DECLARE_SINGLETON(ScriptEngine);

	ScriptParseResult::operator bool() const
	{
		return IsOk();
	}

	void ScriptEngine::RegisterTypes()
	{
		Timer t;

		auto global = GetGlobal();
		ScriptConstructorTypeProcessor processor;
		for (auto func : GetRegisterConstructorFuncs())
			func(0, processor);

		GetRegisterConstructorFuncs().Clear();
// 		auto& types = o2Reflection.GetTypes();
// 		for (auto kv : types)
// 		{
// 			if (kv.second->GetUsage() == Type::Usage::Object)
// 			{
// 				auto objectType = dynamic_cast<ObjectType*>(kv.second);
// 				ScriptValue nspace = GetNameSpace(global, kv.first);
// 				nspace.SetProperty(ScriptValue("New"), ScriptValue(Function<ScriptValue()>([=]() {
// 					auto iobject = objectType->DynamicCastToIObject(objectType->CreateSample());
// 					return iobject->GetScriptValue();
// 				})));
// 			}
// 		}

		mLog->Out("Registered types in " + (String)t.GetDeltaTime() + " seconds");
	}

	void ScriptEngine::RunBuildtinScripts()
	{
		Eval(o2FileSystem.ReadFile(GetBuiltitAssetsPath() + String("Scripts/Math.js")));
	}

	Vector<ScriptEngine::RegisterConstructorFunc>& ScriptEngine::GetRegisterConstructorFuncs()
	{
		static Vector<ScriptEngine::RegisterConstructorFunc> funcs;
		return funcs;
	}

}

#endif // IS_SCRIPTING_SUPPORTED