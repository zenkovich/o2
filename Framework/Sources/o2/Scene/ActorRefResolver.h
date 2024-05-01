#pragma once

#include "o2/Scene/ActorRef.h"
#include "o2/Scene/ComponentRef.h"
#include "o2/Utils/Serialization/DataValue.h"
#include "o2/Utils/Singleton.h"

namespace o2
{
    // -------------------------
    // Actor data node converter
    // -------------------------
    class ActorRefResolver : public Singleton<ActorRefResolver>
    {
    public:
        // Requires to resolve actor reference by actor id
        static void RequireResolve(BaseActorRef& ref, SceneUID actorId);

        // Requires to resolve actor reference by asset id
        static void RequireResolve(BaseActorRef& ref, const UID& assetId);

        // Requires to resolve component reference
        static void RequireResolve(BaseComponentRef& ref, SceneUID actorId, SceneUID id);

        // Requires to resolve component reference
        static void RequireResolve(BaseComponentRef& ref, const UID& assetId, SceneUID id);

        // Requires to remap actor reference
        static void RequireRemap(BaseActorRef& ref);

        // Requires to remap component reference
        static void RequireRemap(BaseComponentRef& ref);

        // Locks references resolving depth
        static void LockResolving(int depth = 1);

        // Unlocks references resolving depth
        static void UnlockResolving(int depth = 1);

        // Resolves references. Works when lock depth is 0
        static void ResolveRefs();

        // Returns is resolver depth locked
        static bool IsLocked();

        // Returns lock depth
        static int GetLockDepth();

        // Remaps required refs 
        static void RemapReferences(const Map<const Actor*, Actor*>& actors, const Map<const Component*, Component*>& components);

        // Called when new actor was created
        static void ActorCreated(Actor* actor);

        // Called when actor id was changed
        static void OnActorIdChanged(Actor* actor, SceneUID prevId);

        // Called when new component was created
        static void ComponentCreated(Component* component);

        // Called when component id was changed
        static void OnComponentIdChanged(Component* component, SceneUID prevId);

        // Called when actor reference was destroyed, removes it from unresolved list
        static void OnActorRefDestroyed(const BaseActorRef* ref);

        // Called when component reference was destroyed, removes it from unresolved list
        static void OnComponentRefDestroyed(const BaseComponentRef* ref);

    protected:
        struct UnresolvedActorRef
        {
            BaseActorRef* target;
            SceneUID      sourceId;

        public:
            UnresolvedActorRef();
            UnresolvedActorRef(BaseActorRef* target, SceneUID actorId);
            bool operator==(const UnresolvedActorRef& other) const;
        };

        struct UnresolvedAssetActorRef
        {
            BaseActorRef* target;
            UID           sourceAssetId;

        public:
            UnresolvedAssetActorRef();
            UnresolvedAssetActorRef(BaseActorRef* target, const UID& assetId);

            bool operator==(const UnresolvedAssetActorRef& other) const;
        };

        struct UnresolvedComponentRef
        {
            BaseComponentRef* target;
            SceneUID          sourceId;

        public:
            UnresolvedComponentRef();
            UnresolvedComponentRef(BaseComponentRef* target, SceneUID id);

            bool operator==(const UnresolvedComponentRef& other) const;
        };

    protected:
        Vector<UnresolvedActorRef>      mUnresolvedActorsRefs;
        Vector<UnresolvedAssetActorRef> mUnresolvedAssetActorsRefs;
        Map<SceneUID, Actor*>           mNewActors;

        Vector<UnresolvedComponentRef> mUnresolvedComponentsRefs;
        Map<SceneUID, Component*>      mNewComponents;

        Vector<BaseActorRef*>     mRemapActorRefs;
        Vector<BaseComponentRef*> mRemapComponentRefs;

        int mLockDepth = 0;

        friend class Actor;
        friend class Component;
        friend class Scene;
    };
}
