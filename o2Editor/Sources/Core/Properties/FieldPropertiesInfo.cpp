#include "stdafx.h"
#include "FieldPropertiesInfo.h"

#include "Core/Properties/IPropertyField.h"
#include "Utils/Basic/IObject.h"
#include "Utils/System/Time/Time.h"
#include "Utils/Debug/Debug.h"

namespace Editor
{
	void FieldPropertiesInfo::Set(const Vector<IObject*>& targets)
	{
		if (targets.IsEmpty())
			return;

		for (auto kv : properties)
		{
			Vector<IAbstractValueProxy*> fieldPointers = targets.Select<IAbstractValueProxy*>([&](IObject* x) 
			{ 
				auto fieldInfo = kv.Key();
				auto type = fieldInfo->GetType();
				return type->GetValueProxy(fieldInfo->GetValuePtrStrong(x)); 
			});

			kv.Value()->SetValueProxy(fieldPointers);
		}
	}

	void FieldPropertiesInfo::Set(const Vector<Pair<IObject*, IObject*>>& targets)
	{
		if (targets.IsEmpty())
			return;

		for (auto kv : properties)
		{
			auto fieldPointers = targets.Select<Pair<IAbstractValueProxy*, IAbstractValueProxy*>>(
				[&](const Pair<IObject*, IObject*>& x)
			{
				auto fieldInfo = kv.Key();
				const Type& type = *fieldInfo->GetOwnerType();
				const ObjectType* objType = dynamic_cast<const ObjectType*>(&type);

				if (objType == nullptr)
					return Pair<IAbstractValueProxy*, IAbstractValueProxy*>(nullptr, nullptr);

				void* firstObjectPtr = objType->DynamicCastFromIObject(x.first);
				void* secondObjectPtr = nullptr;
				if (x.second)
					secondObjectPtr = objType->DynamicCastFromIObject(x.second);

				IAbstractValueProxy* firstValuePtr = fieldInfo->GetType()->GetValueProxy(fieldInfo->GetValuePtrStrong(firstObjectPtr));
				IAbstractValueProxy* secondValuePtr = nullptr;
				if (x.second)
					secondValuePtr = fieldInfo->GetType()->GetValueProxy(fieldInfo->GetValuePtrStrong(secondObjectPtr));

				return Pair<IAbstractValueProxy*, IAbstractValueProxy*>(firstValuePtr, secondValuePtr);
			});

			kv.Value()->SetValueAndPrototypeProxy(fieldPointers);
		}
	}

}