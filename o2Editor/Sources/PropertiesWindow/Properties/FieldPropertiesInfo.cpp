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
				const ObjectType* objType = dynamic_cast<const ObjectType*>(&type);

				if (objType == nullptr)
					return Pair<void*, void*>(nullptr, nullptr);

				void* firstObjectPtr = objType->DynamicCastFromIObject(x.first);
				void* secondObjectPtr = nullptr;
				if (x.second)
					secondObjectPtr = objType->DynamicCastFromIObject(x.second);

				void* firstValuePtr = kv.Key()->GetValuePtrStrong(firstObjectPtr);
				void* secondValuePtr = nullptr;
				if (x.second)
					secondValuePtr = kv.Key()->GetValuePtrStrong(secondObjectPtr);

				return Pair<void*, void*>(firstValuePtr, secondValuePtr);
			});

			kv.Value()->SetValueAndPrototypePtr(fieldPointers, kv.Key()->GetType()->GetUsage() == Type::Usage::Property);
		}
	}

}