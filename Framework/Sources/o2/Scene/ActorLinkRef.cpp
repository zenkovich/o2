#include "o2/stdafx.h"
#include "ActorLinkRef.h"

#include "o2/Scene/Actor.h"
#include "o2/Scene/Scene.h"

namespace o2
{
    BaseActorLinkRef::BaseActorLinkRef()
    {}

    BaseActorLinkRef::BaseActorLinkRef(const BaseActorLinkRef& other)
    {
        if (other.mRequiredResolveData)
        {
            mRequiredResolveData = other.mRequiredResolveData->Clone();
            mRequiredResolveData->RequireResolve(*this);
        }

        ActorRefResolver::RequireRemap(*this);
    }

    BaseActorLinkRef::~BaseActorLinkRef()
    {
        mRequiredResolveData = nullptr;

        ActorRefResolver::OnActorDestroyed(this);
    }

    BaseActorLinkRef& BaseActorLinkRef::operator=(const BaseActorLinkRef& other)
    {
        CopyWithoutRemap(other);
        ActorRefResolver::RequireRemap(*this);

        return *this;
    }

    Actor* BaseActorLinkRef::Get()
    {
        return nullptr;
    }

    const Actor* BaseActorLinkRef::Get() const
    {
        return nullptr;
    }

	void BaseActorLinkRef::Set(Actor* actor)
	{}

	void BaseActorLinkRef::Destroy()
    {
        o2Scene.DestroyActor(Ref(Get()));
    }

    bool BaseActorLinkRef::IsValid() const
    {
        return Get() != nullptr;
    }

    const Type& BaseActorLinkRef::GetActorType() const
    {
        return TypeOf(Actor);
    }

    const Type* BaseActorLinkRef::GetActorTypeStatic()
    {
        return &TypeOf(Actor);
    }

    bool BaseActorLinkRef::EqualsDelta(const BaseActorLinkRef& obj, const BaseActorLinkRef& origin)
    {
        if (obj.Get() == origin.Get())
            return true;

        if (obj.Get() && obj.Get()->mPrototypeLink.Lock().Get() == origin.Get())
            return true;

        return false;
    }

    void BaseActorLinkRef::CopyWithoutRemap(const BaseActorLinkRef& other)
    {}

    void BaseActorLinkRef::OnSerialize(DataValue& node) const
    {
        if (auto actor = Get())
        {
            if (actor->mIsAsset)
                node.AddMember("AssetId") = actor->GetAssetID();
            else
                node.AddMember("ID") = actor->GetID();
        }
    }

    void BaseActorLinkRef::OnDeserialized(const DataValue& node)
    {
        if (auto assetIdNode = node.FindMember("AssetId"))
        {
            auto resolveData = mmake<AssetRequireResolveData>();
            resolveData->uid = (UID)*assetIdNode;
            resolveData->RequireResolve(*this);
            mRequiredResolveData = resolveData;
        }
        else if (auto sceneIdNode = node.FindMember("ID"))
        {
            auto resolveData = mmake<SceneRequireResolveData>();
            resolveData->uid = (SceneUID)*sceneIdNode;
            resolveData->RequireResolve(*this);
            mRequiredResolveData = resolveData;
        }
        else
            Set(nullptr);
    }

    void BaseActorLinkRef::SceneRequireResolveData::RequireResolve(BaseActorLinkRef& ref)
    {
        ActorRefResolver::RequireResolve(ref, uid);
    }

    Ref<BaseActorLinkRef::IRequiredResolveData> BaseActorLinkRef::SceneRequireResolveData::Clone() const
    {
        return mmake<SceneRequireResolveData>(*this);
    }

    void BaseActorLinkRef::AssetRequireResolveData::RequireResolve(BaseActorLinkRef& ref)
    {
        ActorRefResolver::RequireResolve(ref, uid);
    }

    Ref<BaseActorLinkRef::IRequiredResolveData> BaseActorLinkRef::AssetRequireResolveData::Clone() const
    {
        return mmake<AssetRequireResolveData>(*this);
    }
}
// --- META ---

DECLARE_CLASS(o2::BaseActorLinkRef, o2__BaseActorLinkRef);
// --- END META ---
