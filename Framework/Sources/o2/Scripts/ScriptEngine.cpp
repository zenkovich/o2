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
		Function<Pair<ScriptValue, String>(ScriptValue&, const String&)> getNamespace = 
			[getNamespace](ScriptValue& base, const String& path)
		{
			int fnd = path.Find("::");
			if (fnd < 0)
				return Pair<ScriptValue, String>(base, path);

			auto subPath = path.SubStr(0, fnd);
			if (!base.GetProperty(ScriptValue(subPath)))
				base.SetProperty(ScriptValue(subPath), ScriptValue());

			return getNamespace(base.GetProperty(ScriptValue(subPath)), path.SubStr(fnd + 2));
		};

		auto global = GetGlobal();
		auto& types = o2Reflection.GetTypes();
		for (auto kv : types)
		{
			if (kv.second->GetUsage() == Type::Usage::Object)
			{
				auto objectType = dynamic_cast<ObjectType*>(kv.second);
				auto nspace = getNamespace(global, kv.first);
				nspace.first.SetProperty(ScriptValue(nspace.second), ScriptValue(Function<void()>([=]() {
					auto iobject = objectType->DynamicCastToIObject(objectType->CreateSample());
					iobject->GetScriptValue();
				})));
			}
		}
	}

}

#endif // IS_SCRIPTING_SUPPORTED