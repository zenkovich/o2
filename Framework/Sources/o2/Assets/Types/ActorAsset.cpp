#include "o2/stdafx.h"
#include "ActorAsset.h"

#include "o2/Assets/Assets.h"
#include "o2/Scene/Actor.h"

#include "o2/Scripts/ScriptValue.h"

namespace o2
{
    ActorAsset::ActorAsset():
        mActor(mnew Actor(ActorCreateMode::NotInScene))
    {
        mActor->mIsAsset = true;
        mActor->mAssetId = ID();
        mOwnActor = true;
    }

    ActorAsset::ActorAsset(const ActorAsset& other):
        AssetWithDefaultMeta<ActorAsset>(other)
    {
        if (other.mOwnActor)
        {
            mActor = other.mActor->CloneAs<Actor>();
            mActor->mIsAsset = true;
            mActor->mAssetId = ID();
            mOwnActor = true;
        }
        else
        {
            mActor = other.mActor;
            mOwnActor = false;
        }
    }

    ActorAsset::ActorAsset(Actor* actor):
        mActor(actor)
    {
        if (!actor->IsAsset())
        {
            mActor->RemoveFromScene();
            mActor->mIsAsset = true;
            mActor->mAssetId = ID();
        }
        else
            ID() = mActor->mAssetId;

        mOwnActor = false;
    }

    ActorAsset::~ActorAsset()
    {
        if (mOwnActor)
            delete mActor;
    }

    ActorAsset& ActorAsset::operator=(const ActorAsset& other)
    {
        Asset::operator=(other);

        if (mOwnActor)
            delete mActor;
        
        if (other.mOwnActor)
        {
            mActor = other.mActor->CloneAs<Actor>();
            mActor->mIsAsset = true;
            mActor->mAssetId = ID();
            mOwnActor = true;
        }
        else
        {
            mActor = other.mActor;
            mOwnActor = false;
        }

        return *this;
    }

    ActorRef ActorAsset::Instantiate() const
    {
        if (!mActor)
            return nullptr;

        return mActor->CloneAs<Actor>();
    }

    Vector<String> ActorAsset::GetFileExtensions()
    {
        return { "proto" };
    }

    void ActorAsset::OnUIDChanged(const UID& oldUID)
    {
        if (mActor)
            mActor->mAssetId = ID();
    }

    void ActorAsset::OnSerialize(DataValue& node) const
    {
        mActor->mIsAsset = false;
        node["mActor"] = mActor;
        mActor->mIsAsset = true;
    }

    void ActorAsset::OnDeserialized(const DataValue& node)
    {
        mActor = node["mActor"];

        if (mActor)
        {
            mActor->RemoveFromScene();
            mActor->mIsAsset = true;
            mActor->mAssetId = GetUID();
            mOwnActor = true;
        }
    }

    Actor* ActorAsset::GetActor() const
    {
        return mActor;
    }

    void ActorAsset::SetActor(Actor* actor, bool own /*= true*/)
    {
        if (mActor && mOwnActor)
            delete mActor;

        mActor = actor;
        mOwnActor = own;

        if (mActor)
        {
            if (!actor->IsAsset())
            {
                mActor->RemoveFromScene();
                mActor->mIsAsset = true;
                mActor->mAssetId = ID();
            }
            else
                ID() = mActor->mAssetId;
        }
    }

}

DECLARE_TEMPLATE_CLASS(o2::AssetWithDefaultMeta<o2::ActorAsset>);
DECLARE_TEMPLATE_CLASS(o2::DefaultAssetMeta<o2::ActorAsset>);
DECLARE_TEMPLATE_CLASS(o2::Ref<o2::ActorAsset>);
// --- META ---

DECLARE_CLASS(o2::ActorAsset, o2__ActorAsset);
// --- END META ---
