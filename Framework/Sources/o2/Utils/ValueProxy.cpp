#include "o2/stdafx.h"
#include "ValueProxy.h"

namespace o2
{
#if IS_SCRIPTING_SUPPORTED
	ScriptValueProxy::ScriptValueProxy(const ScriptValueProperty& prop) :ScriptValueProperty(prop)
	{}

	ScriptValueProxy::ScriptValueProxy()
	{}

	void ScriptValueProxy::SetValuePtr(void* value)
	{
		GetType().CopyValue(Get().GetContainingObject(), value);
	}

	void ScriptValueProxy::GetValuePtr(void* value) const
	{
		GetType().CopyValue(value, Get().GetContainingObject());
	}

	const Type& ScriptValueProxy::GetType() const
	{
		return *Get().GetObjectContainerType();
	}
#endif
}