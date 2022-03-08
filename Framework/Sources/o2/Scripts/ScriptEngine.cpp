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

	void FixNamespace(String& path)
	{
		path.ReplaceAll("::", "");
		path.ReplaceAll("<", "_");
		path.ReplaceAll(">", "");
		path.ReplaceAll("_o2", "");
	}

	ScriptValue GetNameSpace(ScriptValue& base, const String& path)
	{
		int fnd = -1;
		int braces = 0;
		for (int i = 0; i < path.Length(); i++)
		{
			if (path[i] == '<')
				braces++;

			if (path[i] == '>')
				braces--;

			if (i > 0 && path[i] == ':' && path[i - 1] == ':' && braces == 0)
			{
				fnd = i - 1;
				break;
			}
		}

		if (fnd < 0)
		{
			String fixedPath = path;
			FixNamespace(fixedPath);
			ScriptValue endPath = ScriptValue::EmptyObject();
			base.SetProperty(ScriptValue(fixedPath), endPath);
			return endPath;
		}

		auto subPath = path.SubStr(0, fnd);
		FixNamespace(subPath);
		ScriptValue subPathValue(subPath);
		ScriptValue subPathProp = base.GetProperty(subPathValue);
		if (subPathProp.GetValueType() == ScriptValue::ValueType::Undefined)
		{
			subPathProp = ScriptValue::EmptyObject();
			base.SetProperty(ScriptValue(subPath), subPathProp);
		}

		return GetNameSpace(subPathProp, path.SubStr(fnd + 2));
	}

	void ScriptEngine::RegisterTypes()
	{
		Timer t;

		auto global = GetGlobal();
		auto& types = o2Reflection.GetTypes();
		for (auto kv : types)
		{
			if (kv.second->GetUsage() == Type::Usage::Object)
			{
				auto objectType = dynamic_cast<ObjectType*>(kv.second);
				ScriptValue nspace = GetNameSpace(global, kv.first);
				nspace.SetProperty(ScriptValue("New"), ScriptValue(Function<ScriptValue()>([=]() {
					auto iobject = objectType->DynamicCastToIObject(objectType->CreateSample());
					return iobject->GetScriptValue();
				})));
			}
		}

		mLog->Out("Registered types in " + (String)t.GetDeltaTime() + " seconds");
	}

	void ScriptEngine::RunBuildtinScripts()
	{
		Eval(o2FileSystem.ReadFile(GetBuiltitAssetsPath() + String("Scripts/Math.js")));
	}
}

#endif // IS_SCRIPTING_SUPPORTED