#include "o2/stdafx.h"
#include "ActorRefResolver.h"

#include "o2/Scene/Actor.h"

namespace o2
{
	CREATE_SINGLETON(ActorRefResolver);

	void ActorRefResolver::RequireResolve(ActorRef& ref, SceneUID actorId)
	{
		if (!mInstance)
			return;

		mInstance->mUnresolvedActors.Add(UnresolvedActorRef(&ref, actorId));
	}

	void ActorRefResolver::RequireResolve(ActorRef& ref, const UID& assetId)
	{
		if (!mInstance)
			return;

		mInstance->mUnresolvedActors.Add(UnresolvedActorRef(&ref, assetId));
	}

	void ActorRefResolver::RequireResolve(ComponentRef& ref, SceneUID id)
	{
		if (!mInstance)
			return;

		mInstance->mUnresolvedComponents.Add(UnresolvedComponentRef(&ref, id));
	}

	bool ActorRefResolver::IsLocked()
	{
		if (!mInstance)
			return false;

		return mInstance->mLockDepth != 0;
	}

	void ActorRefResolver::LockResolving()
	{
		if (!mInstance)
			return;

		mInstance->mLockDepth++;
	}

	void ActorRefResolver::UnlockResolving()
	{
		if (!mInstance)
			return;

		mInstance->mLockDepth--;

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

		for (auto def : mInstance->mUnresolvedActors)
		{
			Actor* res = nullptr;
			if (mInstance->mNewActors.TryGetValue(def.sourceActorId, res))
				*def.target = res;
			else
			{
				if (def.isSourceAsset)
					*def.target = o2Scene.GetAssetActorByID(def.sourceAssetId);
				else
					*def.target = o2Scene.GetActorByID(def.sourceActorId);
			}
		}

		for (auto def : mInstance->mUnresolvedComponents)
		{
			Component* res = nullptr;
			if (mInstance->mNewComponents.TryGetValue(def.sourceId, res))
				*def.target = res;
		}

		mInstance->mNewActors.Clear();
		mInstance->mNewComponents.Clear();
		mInstance->mUnresolvedActors.Clear();
		mInstance->mUnresolvedComponents.Clear();
	}

	void ActorRefResolver::ActorCreated(Actor* actor)
	{
		if (!IsSingletonInitialzed())
			return;

		if (mInstance->mLockDepth < 1)
			return;

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

	bool ActorRefResolver::UnresolvedActorRef::operator==(const UnresolvedActorRef& other) const
	{
		return target == other.target;
	}

	ActorRefResolver::UnresolvedActorRef::UnresolvedActorRef(ActorRef* target, const UID& assetId):
		target(target), isSourceAsset(true), sourceAssetId(assetId)
	{}

	ActorRefResolver::UnresolvedActorRef::UnresolvedActorRef(ActorRef* target, SceneUID actorId) :
		target(target), isSourceAsset(false), sourceActorId(actorId)
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

}
