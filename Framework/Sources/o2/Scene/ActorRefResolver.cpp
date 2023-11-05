#include "o2/stdafx.h"
#include "ActorRefResolver.h"

#include "o2/Scene/Actor.h"
#include "o2/Scene/Scene.h"

namespace o2
{
    CREATE_SINGLETON(ActorRefResolver);

    void ActorRefResolver::RequireResolve(ActorRef& ref, SceneUID actorId)
    {
        if (!mInstance)
            return;

        if (IsLocked())
            mInstance->mUnresolvedActorsRefs.Add(UnresolvedActorRef(&ref, actorId));
        else
            ref = o2Scene.GetActorByID(actorId);
    }

    void ActorRefResolver::RequireResolve(ActorRef& ref, const UID& assetId)
    {
        if (!mInstance)
            return;

        if (IsLocked())
            mInstance->mUnresolvedAssetActorsRefs.Add(UnresolvedAssetActorRef(&ref, assetId));
        else
            ref = o2Scene.GetAssetActorByID(assetId);
    }

    void ActorRefResolver::RequireResolve(ComponentRef& ref, SceneUID actorId, SceneUID id)
    {
        if (!mInstance)
            return;

        if (IsLocked())
            mInstance->mUnresolvedComponentsRefs.Add(UnresolvedComponentRef(&ref, id));
        else
        {
            if (auto actor = o2Scene.GetActorByID(actorId))
                ref = actor->GetComponent(id);
        }
    }

    void ActorRefResolver::RequireResolve(ComponentRef& ref, const UID& assetId, SceneUID id)
    {
        if (!mInstance)
            return;

        if (IsLocked())
            mInstance->mUnresolvedComponentsRefs.Add(UnresolvedComponentRef(&ref, id));
        else
        {
            if (auto actor = o2Scene.GetAssetActorByID(assetId))
                ref = actor->GetComponent(id);
        }
    }

    bool ActorRefResolver::IsLocked()
    {
        if (!mInstance)
            return false;

        return mInstance->mLockDepth != 0;
    }

    int ActorRefResolver::GetLockDepth()
    {
        if (!mInstance)
            return 0;

        return mInstance->mLockDepth;
    }

    void ActorRefResolver::RemapReferences(const Map<const Actor*, Actor*>& actors,
                                           const Map<const Component*, Component*>& components)
    {
        if (!mInstance)
            return;

        for (auto ref : mInstance->mRemapActorRefs)
        {
            Actor* res = nullptr;
            if (actors.TryGetValue(ref->mActor, res))
                ref->CopyWithoutRemap(res);
        }

        for (auto ref : mInstance->mRemapComponentRefs)
        {
            Component* res = nullptr;
            if (components.TryGetValue(ref->mComponent, res))
                ref->CopyWithoutRemap(res);
        }

        mInstance->mRemapActorRefs.Clear();
        mInstance->mRemapComponentRefs.Clear();
    }

    void ActorRefResolver::RequireRemap(ActorRef& ref)
    {
        if (!mInstance)
            return;

        if (!IsLocked())
            return;

        mInstance->mRemapActorRefs.Add(&ref);
    }

    void ActorRefResolver::RequireRemap(ComponentRef& ref)
    {
        if (!mInstance)
            return;

        if (!IsLocked())
            return;

        mInstance->mRemapComponentRefs.Add(&ref);
    }

    void ActorRefResolver::LockResolving(int depth /*= 1*/)
    {
        if (!mInstance)
            return;

        mInstance->mLockDepth += depth;
    }

    void ActorRefResolver::UnlockResolving(int depth /*= 1*/)
    {
        if (!mInstance)
            return;

        mInstance->mLockDepth -= depth;

        if (mInstance->mLockDepth == 0)
            ResolveRefs();

        if (mInstance->mLockDepth < 0)
        {
            o2Debug.LogWarning("ActorRefResolver lock depth is less than zero. Something going wrong");
            mInstance->mLockDepth = 0;
        }
    }

    void ActorRefResolver::ResolveRefs()
    {
        if (!mInstance)
            return;

        if (mInstance->mLockDepth > 0)
            return;

        for (auto def : mInstance->mUnresolvedActorsRefs)
        {
            Actor* res = nullptr;
            if (mInstance->mNewActors.TryGetValue(def.sourceId, res))
                *def.target = res;
            else
                *def.target = o2Scene.GetActorByID(def.sourceId);
        }

        for (auto def : mInstance->mUnresolvedAssetActorsRefs)
            *def.target = o2Scene.GetAssetActorByID(def.sourceAssetId);

        for (auto def : mInstance->mUnresolvedComponentsRefs)
        {
            Component* res = nullptr;
            if (mInstance->mNewComponents.TryGetValue(def.sourceId, res))
                *def.target = res;
        }

        mInstance->mNewActors.Clear();
        mInstance->mNewComponents.Clear();
        mInstance->mUnresolvedActorsRefs.Clear();
        mInstance->mUnresolvedComponentsRefs.Clear();
    }

    void ActorRefResolver::ActorCreated(Actor* actor)
    {
        if (!IsSingletonInitialzed())
            return;

        if (mInstance->mLockDepth < 1)
            return;

        mInstance->mNewActors[actor->GetID()] = actor;
    }

    void ActorRefResolver::OnActorIdChanged(Actor* actor, SceneUID prevId)
    {
        if (!IsSingletonInitialzed())
            return;

        if (mInstance->mLockDepth < 1)
            return;

        mInstance->mNewActors.Remove(prevId);
        mInstance->mNewActors[actor->GetID()] = actor;
    }

    void ActorRefResolver::ComponentCreated(Component* component)
    {
        if (!IsSingletonInitialzed())
            return;

        if (mInstance->mLockDepth < 1)
            return;

        mInstance->mNewComponents[component->GetID()] = component;
    }

    void ActorRefResolver::OnComponentIdChanged(Component* component, SceneUID prevId)
    {
        if (!IsSingletonInitialzed())
            return;

        if (mInstance->mLockDepth < 1)
            return;

        mInstance->mNewComponents.Remove(prevId);
        mInstance->mNewComponents[component->GetID()] = component;
    }

    void ActorRefResolver::OnActorRefDestroyed(const ActorRef* ref)
    {
        mInstance->mUnresolvedActorsRefs.RemoveFirst([&](auto& x) { return x.target == ref; });
        mInstance->mUnresolvedAssetActorsRefs.RemoveFirst([&](auto& x) { return x.target == ref; });

        mInstance->mRemapActorRefs.Remove(const_cast<ActorRef*>(ref));
    }

    void ActorRefResolver::OnComponentRefDestroyed(const ComponentRef* ref)
    {
        mInstance->mUnresolvedComponentsRefs.RemoveFirst([&](auto& x) { return x.target == ref; });
        mInstance->mRemapComponentRefs.Remove(const_cast<ComponentRef*>(ref));
    }

    bool ActorRefResolver::UnresolvedActorRef::operator==(const UnresolvedActorRef& other) const
    {
        return target == other.target;
    }

    ActorRefResolver::UnresolvedActorRef::UnresolvedActorRef(ActorRef* target, SceneUID actorId) :
        target(target), sourceId(actorId)
    {}

    ActorRefResolver::UnresolvedActorRef::UnresolvedActorRef()
    {}

    ActorRefResolver::UnresolvedComponentRef::UnresolvedComponentRef()
    {}

    ActorRefResolver::UnresolvedComponentRef::UnresolvedComponentRef(ComponentRef* target, SceneUID id):
        target(target), sourceId(id)
    {}

    bool ActorRefResolver::UnresolvedComponentRef::operator==(const UnresolvedComponentRef& other) const
    {
        return target == other.target;
    }

    ActorRefResolver::UnresolvedAssetActorRef::UnresolvedAssetActorRef()
    {}

    ActorRefResolver::UnresolvedAssetActorRef::UnresolvedAssetActorRef(ActorRef* target, const UID& assetId):
        target(target), sourceAssetId(assetId)
    {}

    bool ActorRefResolver::UnresolvedAssetActorRef::operator==(const UnresolvedAssetActorRef& other) const
    {
        return target == other.target;
    }

}
