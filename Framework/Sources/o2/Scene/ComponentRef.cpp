#include "o2/stdafx.h"
#include "ComponentRef.h"

#include "o2/Scene/Scene.h"

namespace o2
{
    BaseComponentRef::BaseComponentRef()
    {}

    BaseComponentRef::BaseComponentRef(const BaseComponentRef& other)
    {
        if (other.mRequiredResolveData)
        {
            mRequiredResolveData = other.mRequiredResolveData->Clone();
            mRequiredResolveData->RequireResolve(*this);
        }

        ActorRefResolver::RequireRemap(*this);
    }

    BaseComponentRef::~BaseComponentRef()
    {
        mRequiredResolveData = nullptr;
        ActorRefResolver::OnComponentRefDestroyed(this);
    }

    void BaseComponentRef::Set(Component* component)
    {}

    Component* BaseComponentRef::Get()
    {
        return nullptr;
    }

    const Component* BaseComponentRef::Get() const
    {
        return nullptr;
    }

    void BaseComponentRef::Destroy()
    {
        o2Scene.DestroyComponent(Ref(Get()));
    }

    const Type& BaseComponentRef::GetComponentType() const
    {
        return TypeOf(Component);
    }

    const Type* BaseComponentRef::GetComponentTypeStatic()
    {
        return &TypeOf(Component);
    }

    bool BaseComponentRef::EqualsDelta(const BaseComponentRef& obj, const BaseComponentRef& origin)
    {
        if (obj.Get() == origin.Get())
            return true;

        if (obj.Get() && obj.Get()->mPrototypeLink.Lock().Get() == origin.Get())
            return true;

        return false;
    }

    void BaseComponentRef::CopyWithoutRemap(const BaseComponentRef& other)
    {}

    void BaseComponentRef::OnSerialize(DataValue& node) const
    {
        if (auto component = Get())
        {
            auto actor = component->mOwner.Lock();
            if (actor->mIsAsset)
                node.AddMember("AssetID") = actor->GetAssetID();
            else 
                node.AddMember("ActorID") = actor->GetID();

            node.AddMember("ComponentId") = component->mId;
        }
    }

    void BaseComponentRef::OnDeserialized(const DataValue& node)
    {
        if (auto assetIdNode = node.FindMember("AssetId"))
        {
            auto resolveData = mmake<AssetRequireResolveData>();
            resolveData->uid = (UID)*assetIdNode;
            resolveData->componentUID = (SceneUID)node.GetMember("ComponentId");
            resolveData->RequireResolve(*this);
            mRequiredResolveData = resolveData;
        }
        else if (auto sceneIdNode = node.FindMember("ActorID"))
        {
            auto resolveData = mmake<SceneRequireResolveData>();
            resolveData->uid = (SceneUID)*sceneIdNode;
            resolveData->componentUID = (SceneUID)node.GetMember("ComponentId");
            resolveData->RequireResolve(*this);
            mRequiredResolveData = resolveData;
        }
        else 
            Set(nullptr);
    }

    void BaseComponentRef::SceneRequireResolveData::RequireResolve(BaseComponentRef& ref)
    {
        ActorRefResolver::RequireResolve(ref, uid, componentUID);
    }

    Ref<BaseComponentRef::IRequiredResolveData> BaseComponentRef::SceneRequireResolveData::Clone() const
    {
        return mmake<SceneRequireResolveData>(*this);
    }

    void BaseComponentRef::AssetRequireResolveData::RequireResolve(BaseComponentRef& ref)
    {
        ActorRefResolver::RequireResolve(ref, uid, componentUID);
    }

    Ref<BaseComponentRef::IRequiredResolveData> BaseComponentRef::AssetRequireResolveData::Clone() const
    {
        return mmake<AssetRequireResolveData>(*this);
    }

}
// --- META ---

DECLARE_CLASS(o2::Ref<Component>, o2__ComponentRef);
// --- END META ---
