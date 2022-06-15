#include "o2/stdafx.h"
#include "ValueProxy.h"

namespace o2
{
#if IS_SCRIPTING_SUPPORTED
	ScriptValueProxy::ScriptValueProxy(IScriptValueProperty* prop) :
		scriptProperty(prop)
	{}

	ScriptValueProxy::ScriptValueProxy()
	{}

	ScriptValueProxy::~ScriptValueProxy()
	{
		if (scriptProperty)
			delete scriptProperty;
	}

	void ScriptValueProxy::SetValuePtr(void* value)
	{
		GetType().CopyValue(scriptProperty->Get().GetContainingObject(), value);
	}

	void ScriptValueProxy::GetValuePtr(void* value) const
	{
		GetType().CopyValue(value, scriptProperty->Get().GetContainingObject());
	}

	const Type& ScriptValueProxy::GetType() const
	{
		return *scriptProperty->Get().GetObjectContainerType();
	}
#endif
}