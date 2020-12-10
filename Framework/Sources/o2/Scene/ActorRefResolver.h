#pragma once

#include "o2/Scene/ComponentRef.h"
#include "o2/Scene/Scene.h"
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
		static void RequireResolve(ActorRef& ref, SceneUID actorId);

		// Requires to resolve actor reference by asset id
		static void RequireResolve(ActorRef& ref, const UID& assetId);

		// Requires to resolve component reference
		static void RequireResolve(ComponentRef& ref, SceneUID actorId, SceneUID id);

		// Requires to resolve component reference
		static void RequireResolve(ComponentRef& ref, const UID& assetId, SceneUID id);

		// Requires to remap actor reference
		static void RequireRemap(ActorRef& ref);

		// Requires to remap component reference
		static void RequireRemap(ComponentRef& ref);

		// Locks references resolving depth
		static void LockResolving();

		// Unlocks references resolving depth
		static void UnlockResolving();

		// Resolves references. Works when lock depth is 0
		static void ResolveRefs();

		// Returns is resolver depth locked
		static bool IsLocked();

		// Remaps required refs 
		static void RemapReferences(const Map<const Actor*, Actor*>& actors, const Map<const Component*, Component*>& components);

		// It is called when new actor was created
		static void ActorCreated(Actor* actor);

		// It is called when actor id was changed
		static void OnActorIdChanged(Actor* actor, SceneUID prevId);

		// It is called when new component was created
		static void ComponentCreated(Component* component);

		// It is called when component id was changed
		static void OnComponentIdChanged(Component* component, SceneUID prevId);

	protected:
		struct UnresolvedActorRef
		{
			ActorRef* target;
			SceneUID  sourceId;

		public:
			UnresolvedActorRef();
			UnresolvedActorRef(ActorRef* target, SceneUID actorId);
			bool operator==(const UnresolvedActorRef& other) const;
		};

		struct UnresolvedAssetActorRef
		{
			ActorRef* target;
			UID       sourceAssetId;

		public:
			UnresolvedAssetActorRef();
			UnresolvedAssetActorRef(ActorRef* target, const UID& assetId);

			bool operator==(const UnresolvedAssetActorRef& other) const;
		};

		struct UnresolvedComponentRef
		{
			ComponentRef* target;
			SceneUID      sourceId;

		public:
			UnresolvedComponentRef();
			UnresolvedComponentRef(ComponentRef* target, SceneUID id);

			bool operator==(const UnresolvedComponentRef& other) const;
		};

	protected:
		Vector<UnresolvedActorRef>      mUnresolvedActors;
		Vector<UnresolvedAssetActorRef> mUnresolvedAssetActors;
		Map<SceneUID, Actor*>           mNewActors;

		Vector<UnresolvedComponentRef> mUnresolvedComponents;
		Map<SceneUID, Component*>      mNewComponents;

		Vector<ActorRef*>     mRemapActorRefs;
		Vector<ComponentRef*> mRemapComponentRefs;

		int mLockDepth = 0;

		friend class Actor;
		friend class Component;
		friend class Scene;

		friend class ActorRef;
	};
}
