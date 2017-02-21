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

// 		auto fields = targets[0]->GetType().GetFields();
// 		for (auto fieldInfo : fields)
// 		{
// 			Vector<void*> fieldPointers = targets.Select<void*>(
// 				[&](IObject* x) { return fieldInfo->GetValuePtrStrong(x); });
// 
// 			IPropertyField* propertyField;
// 			if (properties.TryGetValue(fieldInfo, propertyField))
// 				propertyField->SetValuePtr(fieldPointers, fieldInfo->GetType()->GetUsage() == Type::Usage::Property);
// 		}
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
				return Pair<void*, void*>(kv.Key()->GetValuePtrStrong(x.first),
										  x.second ? kv.Key()->GetValuePtrStrong(x.second) : nullptr);
			});

			kv.Value()->SetValueAndPrototypePtr(fieldPointers, kv.Key()->GetType()->GetUsage() == Type::Usage::Property);
		}

// 		auto fields = targets[0].first->GetType().GetFields();
// 		for (auto fieldInfo : fields)
// 		{
// 			Vector<Pair<void*, void*>> fieldPointers = targets.Select<Pair<void*, void*>>(
// 				[&](const Pair<IObject*, IObject*>& x) 
// 			{ 
// 				return Pair<void*, void*>(fieldInfo->GetValuePtrStrong(x.first),
// 										  x.second ? fieldInfo->GetValuePtrStrong(x.second) : nullptr); 
// 			});
// 
// 			IPropertyField* propertyField;
// 			if (properties.TryGetValue(fieldInfo, propertyField))
// 				propertyField->SetValueAndPrototypePtr(fieldPointers, fieldInfo->GetType()->GetUsage() == Type::Usage::Property);
// 		}
	}

}