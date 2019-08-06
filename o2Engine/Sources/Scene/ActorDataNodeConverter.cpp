#include "stdafx.h"
#include "ActorDataNodeConverter.h"

#include "Scene/Actor.h"
#include "Scene/Scene.h"

namespace o2
{
	CREATE_SINGLETON(ActorDataNodeConverter);

	void ActorDataNodeConverter::ToData(const Actor* object, DataNode& data)
	{
		if (object)
		{
			if (object->mIsAsset)
				*data.AddNode("AssetId") = object->GetAssetID();
			else if (object->IsOnScene() || object->IsAsset())
				*data.AddNode("ID") = object->GetID();
			else
				*data.AddNode("Data") = object ? object->Serialize() : (String)"null";
		}
	}

	void ActorDataNodeConverter::FromData(Actor*& object, const DataNode& data)
	{
		if (auto assetIdNode = data.GetNode("AssetId"))
		{
			UID assetId = *assetIdNode;
			object = o2Scene.GetAssetActorByID(assetId);

			if (!object)
				mUnresolvedActors.Add(ActorDef(&object, assetId));
		}
		else if (auto sceneIdNode = data.GetNode("ID"))
		{
			if (mLockDepth == 0)
				object = o2Scene.GetActorByID(*sceneIdNode);

			if (!object)
				mUnresolvedActors.Add(ActorDef(&object, (SceneUID)*sceneIdNode));
		}
		else if (auto dataNode = data.GetNode("Data"))
		{
			if (dataNode->Data() == "null")
				object = nullptr;
			else
			{
				object = mnew Actor(ActorCreateMode::NotInScene);
				object->Deserialize(*dataNode);
			}
		}
		else object = nullptr;
	}

	void ActorDataNodeConverter::LockPointersResolving()
	{
		mLockDepth++;
	}

	void ActorDataNodeConverter::UnlockPointersResolving()
	{
		mLockDepth--;

		if (mLockDepth < 0)
		{
			o2Debug.LogWarning("ActorDataNodeConverter lock depth is less than zero. Something going wrong");
			mLockDepth = 0;
		}
	}

	void ActorDataNodeConverter::ResolvePointers()
	{
		if (mLockDepth > 0)
			return;

		for (auto def : mUnresolvedActors)
		{
			*def.target = mNewActors.FindMatch([&](Actor* x) { return x->GetID() == def.actorId; });

			if (!*def.target)
			{
				if (def.isAsset)
					*def.target = o2Scene.GetAssetActorByID(def.assetId);
				else
					*def.target = o2Scene.GetActorByID(def.actorId);
			}
		}

		mNewActors.Clear();
		mUnresolvedActors.Clear();
	}

	void ActorDataNodeConverter::ActorCreated(Actor* actor)
	{
		if (!IsSingletonInitialzed())
			return;

		if (mInstance->mLockDepth < 1)
			return;

		mInstance->mNewActors.Add(actor);
	}
}