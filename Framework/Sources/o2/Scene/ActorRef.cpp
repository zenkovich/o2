#include "o2/stdafx.h"
#include "ActorRef.h"

#include "o2/Scene/Actor.h"
#include "o2/Scene/Scene.h"

namespace o2
{
    BaseActorRef::BaseActorRef()
    {}

    BaseActorRef::BaseActorRef(const BaseActorRef& other)
    {
        if (other.mRequiredResolveData)
        {
            mRequiredResolveData = other.mRequiredResolveData->Clone();
            mRequiredResolveData->RequireResolve(*this);
        }

        ActorRefResolver::RequireRemap(*this);
    }

    BaseActorRef::~BaseActorRef()
    {
        mRequiredResolveData = nullptr;

        ActorRefResolver::OnActorRefDestroyed(this);
    }

    BaseActorRef& BaseActorRef::operator=(const BaseActorRef& other)
    {
        CopyWithoutRemap(other);
        ActorRefResolver::RequireRemap(*this);

        return *this;
    }

    Actor* BaseActorRef::Get()
    {
        return nullptr;
    }

    const Actor* BaseActorRef::Get() const
    {
        return nullptr;
    }

	void BaseActorRef::Set(Actor* actor)
	{}

	void BaseActorRef::Destroy()
    {
        o2Scene.DestroyActor(Ref(Get()));
    }

    bool BaseActorRef::IsValid() const
    {
        return Get() != nullptr;
    }

    const Type& BaseActorRef::GetActorType() const
    {
        return TypeOf(Actor);
    }

    const Type* BaseActorRef::GetActorTypeStatic()
    {
        return &TypeOf(Actor);
    }

    bool BaseActorRef::EqualsDelta(const BaseActorRef& obj, const BaseActorRef& origin)
    {
        if (obj.Get() == origin.Get())
            return true;

        if (obj.Get() && obj.Get()->mPrototypeLink.Get() == origin.Get())
            return true;

        return false;
    }

    void BaseActorRef::CopyWithoutRemap(const BaseActorRef& other)
    {}

    void BaseActorRef::OnSerialize(DataValue& node) const
    {
        if (auto actor = Get())
        {
            if (actor->mIsAsset)
                node.AddMember("AssetId") = actor->GetAssetID();
            else
                node.AddMember("ID") = actor->GetID();
        }
    }

    void BaseActorRef::OnDeserialized(const DataValue& node)
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

    void BaseActorRef::SceneRequireResolveData::RequireResolve(BaseActorRef& ref)
    {
        ActorRefResolver::RequireResolve(ref, uid);
    }

    Ref<BaseActorRef::IRequiredResolveData> BaseActorRef::SceneRequireResolveData::Clone() const
    {
        return mmake<SceneRequireResolveData>(*this);
    }

    void BaseActorRef::AssetRequireResolveData::RequireResolve(BaseActorRef& ref)
    {
        ActorRefResolver::RequireResolve(ref, uid);
    }

    Ref<BaseActorRef::IRequiredResolveData> BaseActorRef::AssetRequireResolveData::Clone() const
    {
        return mmake<AssetRequireResolveData>(*this);
    }
}

DECLARE_TEMPLATE_CLASS(o2::Ref<o2::Actor>);
// --- META ---

DECLARE_CLASS(o2::BaseActorRef, o2__BaseActorRef);
// --- END META ---
