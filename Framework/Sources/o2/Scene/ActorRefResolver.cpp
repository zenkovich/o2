#include "o2/stdafx.h"
#include "ActorRefResolver.h"

#include "o2/Scene/Actor.h"
#include "o2/Scene/ActorRef.h"
#include "o2/Scene/Scene.h"

namespace o2
{
    CREATE_SINGLETON(ActorRefResolver);

    void ActorRefResolver::RequireResolve(BaseActorRef& ref, SceneUID actorId)
    {
        if (!mInstance)
            return;

        if (IsLocked())
            mInstance->mUnresolvedActorsRefs.Add(UnresolvedActorRef(&ref, actorId));
        else
            ref.Set(o2Scene.GetActorByID(actorId).Get());
    }

    void ActorRefResolver::RequireResolve(BaseActorRef& ref, const UID& assetId)
    {
        if (!mInstance)
            return;

        if (IsLocked())
            mInstance->mUnresolvedAssetActorsRefs.Add(UnresolvedAssetActorRef(&ref, assetId));
        else
            ref.Set(o2Scene.GetAssetActorByID(assetId).Get());
    }

    void ActorRefResolver::RequireResolve(BaseComponentRef& ref, SceneUID actorId, SceneUID id)
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

    void ActorRefResolver::RequireResolve(BaseComponentRef& ref, const UID& assetId, SceneUID id)
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

        for (auto& ref : mInstance->mRemapActorRefs)
        {
            Actor* res = nullptr;
            if (actors.TryGetValue(ref->Get(), res))
            {
                Ref<Actor> resRef(res);
                //ref->CopyWithoutRemap(resRef);
            }
        }

        for (auto& ref : mInstance->mRemapComponentRefs)
        {
            Component* res = nullptr;
            if (components.TryGetValue(ref->Get(), res))
                ref->CopyWithoutRemap(Ref(res));
        }

        mInstance->mRemapActorRefs.Clear();
        mInstance->mRemapComponentRefs.Clear();
    }

    void ActorRefResolver::RequireRemap(BaseActorRef& ref)
    {
        if (!mInstance)
            return;

        if (!IsLocked())
            return;

        mInstance->mRemapActorRefs.Add(&ref);
    }

    void ActorRefResolver::RequireRemap(BaseComponentRef& ref)
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

        for (auto& def : mInstance->mUnresolvedActorsRefs)
        {
            Actor* res = nullptr;
            if (mInstance->mNewActors.TryGetValue(def.sourceId, res))
                def.target->Set(res);
            else
                def.target->Set(o2Scene.GetActorByID(def.sourceId).Get());
        }

        for (auto& def : mInstance->mUnresolvedAssetActorsRefs)
            def.target->Set(o2Scene.GetAssetActorByID(def.sourceAssetId).Get());

        for (auto& def : mInstance->mUnresolvedComponentsRefs)
        {
            Component* res = nullptr;
            if (mInstance->mNewComponents.TryGetValue(def.sourceId, res))
                def.target->Set(res);
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

    void ActorRefResolver::OnActorRefDestroyed(const BaseActorRef* ref)
    {
        mInstance->mUnresolvedActorsRefs.RemoveFirst([&](auto& x) { return x.target == ref; });
        mInstance->mUnresolvedAssetActorsRefs.RemoveFirst([&](auto& x) { return x.target == ref; });

        mInstance->mRemapActorRefs.Remove(const_cast<BaseActorRef*>(ref));
    }

    void ActorRefResolver::OnComponentRefDestroyed(const BaseComponentRef* ref)
    {
        mInstance->mUnresolvedComponentsRefs.RemoveFirst([&](auto& x) { return x.target == ref; });
        mInstance->mRemapComponentRefs.Remove(const_cast<BaseComponentRef*>(ref));
    }

    bool ActorRefResolver::UnresolvedActorRef::operator==(const UnresolvedActorRef& other) const
    {
        return target == other.target;
    }

    ActorRefResolver::UnresolvedActorRef::UnresolvedActorRef(BaseActorRef* target, SceneUID actorId) :
        target(target), sourceId(actorId)
    {}

    ActorRefResolver::UnresolvedActorRef::UnresolvedActorRef()
    {}

    ActorRefResolver::UnresolvedComponentRef::UnresolvedComponentRef()
    {}

    ActorRefResolver::UnresolvedComponentRef::UnresolvedComponentRef(BaseComponentRef* target, SceneUID id):
        target(target), sourceId(id)
    {}

    bool ActorRefResolver::UnresolvedComponentRef::operator==(const UnresolvedComponentRef& other) const
    {
        return target == other.target;
    }

    ActorRefResolver::UnresolvedAssetActorRef::UnresolvedAssetActorRef()
    {}

    ActorRefResolver::UnresolvedAssetActorRef::UnresolvedAssetActorRef(BaseActorRef* target, const UID& assetId):
        target(target), sourceAssetId(assetId)
    {}

    bool ActorRefResolver::UnresolvedAssetActorRef::operator==(const UnresolvedAssetActorRef& other) const
    {
        return target == other.target;
    }

}
