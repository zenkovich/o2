#include "o2Editor/stdafx.h"
#include "PropertiesContext.h"

#include "o2/Utils/Basic/IObject.h"
#include "o2/Utils/Debug/Debug.h"
#include "o2/Utils/System/Time/Time.h"
#include "o2Editor/Core/Properties/IPropertyField.h"

namespace Editor
{

    void PropertiesContext::Invalidate()
    {
        targets.Clear();
    }

    void PropertiesContext::Set(const Vector<Pair<Ref<IObject>, Ref<IObject>>>& targets, bool force)
    {
        if (this->targets == targets && !force)
        {
            Refresh();
            return;
        }

        this->targets = targets;

        if (targets.IsEmpty())
            return;

        for (auto& kv : properties)
        {
            auto fieldPointers = targets.Convert<Pair<Ref<IAbstractValueProxy>, Ref<IAbstractValueProxy>>>(
                [&](const Pair<Ref<IObject>, Ref<IObject>>& x)
                {
                    auto fieldInfo = kv.first;
                    const Type& type = *fieldInfo->GetOwnerType();
                    const ObjectType* objType = DynamicCast<const ObjectType>(type);

                    if (objType == nullptr)
                        return Pair<Ref<IAbstractValueProxy>, Ref<IAbstractValueProxy>>(nullptr, nullptr);

                    void* firstObjectPtr = objType->DynamicCastFromIObject(x.first.Get());
                    void* secondObjectPtr = nullptr;
                    if (x.second)
                        secondObjectPtr = objType->DynamicCastFromIObject(x.second.Get());

                    Ref<IAbstractValueProxy> firstValuePtr = fieldInfo->GetType()->GetValueProxy(fieldInfo->GetValuePtrStrong(firstObjectPtr));
                    Ref<IAbstractValueProxy> secondValuePtr = nullptr;
                    if (x.second)
                        secondValuePtr = fieldInfo->GetType()->GetValueProxy(fieldInfo->GetValuePtrStrong(secondObjectPtr));

                    return Pair<Ref<IAbstractValueProxy>, Ref<IAbstractValueProxy>>(firstValuePtr, secondValuePtr);
                });

            kv.second->SetValueAndPrototypeProxy(fieldPointers);
        }
    }

    void PropertiesContext::Refresh()
    {
        for (auto& kv : properties)
            kv.second->Refresh();
    }

    bool PropertiesContext::IsBuiltWIthPrivateProperties() const
    {
        return builtWithPrivateProperties;
    }

    const Map<const FieldInfo*, Ref<IPropertyField>>& PropertiesContext::GetProperties() const
    {
        return properties;
    }

}
