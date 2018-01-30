#include "stdafx.h"
#include "FieldPropertiesInfo.h"

#include "PropertiesWindow/Properties/IPropertyField.h"
#include "Utils/IObject.h"
#include "Utils/Time.h"
#include "Utils/Debug.h"

namespace Editor
{
	void FieldPropertiesInfo::Set(const Vector<IObject*>& targets)
	{
		if (targets.IsEmpty())
			return;

		for (auto kv : properties)
		{
			Vector<void*> fieldPointers = targets.Select<void*>(
				[&](IObject* x) { return kv.Key()->GetValuePtrStrong(x); });

			kv.Value()->SetValuePtr(fieldPointers, kv.Key()->GetType()->GetUsage() == Type::Usage::Property);
		}
	}

	void FieldPropertiesInfo::Set(const Vector<Pair<IObject*, IObject*>>& targets)
	{
		if (targets.IsEmpty())
			return;

		for (auto kv : properties)
		{
			Vector<Pair<void*, void*>> fieldPointers = targets.Select<Pair<void*, void*>>(
				[&](const Pair<IObject*, IObject*>& x)
			{
				const Type& type = *kv.Key()->GetOwnerType();
				return Pair<void*, void*>(type.DynamicCast(kv.Key()->GetValuePtrStrong(x.first), x.first->GetType()),
										  x.second ? type.DynamicCast(kv.Key()->GetValuePtrStrong(x.second), x.second->GetType()) : nullptr);
			});

			kv.Value()->SetValueAndPrototypePtr(fieldPointers, kv.Key()->GetType()->GetUsage() == Type::Usage::Property);
		}
	}

}