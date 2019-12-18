#include "stdafx.h"
#include "PropertiesContext.h"

#include "Core/Properties/IPropertyField.h"
#include "Utils/Basic/IObject.h"
#include "Utils/System/Time/Time.h"
#include "Utils/Debug/Debug.h"

namespace Editor
{
	void PropertiesContext::Set(const Vector<Pair<IObject*, IObject*>>& targets)
	{
		mTargets = targets;

		if (targets.IsEmpty())
			return;

		for (auto& kv : mProperties)
		{
			auto fieldPointers = targets.Select<Pair<IAbstractValueProxy*, IAbstractValueProxy*>>(
				[&](const Pair<IObject*, IObject*>& x)
			{
				auto fieldInfo = kv.first;
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

			kv.second->SetValueAndPrototypeProxy(fieldPointers);
		}
	}

	bool PropertiesContext::IsBuiltWIthPrivateProperties() const
	{
		return mBuildWithPrivateProperties;
	}

	const Map<const FieldInfo*, IPropertyField*>& PropertiesContext::GetProperties() const
	{
		return mProperties;
	}

}
