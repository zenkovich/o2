#include "o2/stdafx.h"
#include "ActorDataValueConverter.h"

#include "o2/Scene/Actor.h"
#include "o2/Scene/Scene.h"

namespace o2
{
	CREATE_SINGLETON(ActorDataValueConverter);

	void ActorDataValueConverter::ToData(const Actor* object, DataValue& data)
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

	void ActorDataValueConverter::FromData(Actor*& object, const DataValue& data)
	{
		if (auto assetIdNode = data.GetMember("AssetId"))
		{
			UID assetId = *assetIdNode;
			object = o2Scene.GetAssetActorByID(assetId);

			if (!object)
				mUnresolvedActors.Add(ActorDef(&object, assetId));
		}
		else if (auto sceneIdNode = data.GetMember("ID"))
		{
			if (mLockDepth == 0)
				object = o2Scene.GetActorByID(*sceneIdNode);

			if (!object)
				mUnresolvedActors.Add(ActorDef(&object, (SceneUID)*sceneIdNode));
		}
		else if (auto DataValue = data.GetMember("Data"))
		{
			if (DataValue->Data() == "null")
				object = nullptr;
			else
			{
				object = mnew Actor(ActorCreateMode::NotInScene);
				object->Deserialize(*DataValue);
			}
		}
		else object = nullptr;
	}

	void ActorDataValueConverter::LockPointersResolving()
	{
		mLockDepth++;
	}

	void ActorDataValueConverter::UnlockPointersResolving()
	{
		mLockDepth--;

		if (mLockDepth < 0)
		{
			o2Debug.LogWarning("ActorDataValueConverter lock depth is less than zero. Something going wrong");
			mLockDepth = 0;
		}
	}

	void ActorDataValueConverter::ResolvePointers()
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

	void ActorDataValueConverter::ActorCreated(Actor* actor)
	{
		if (!IsSingletonInitialzed())
			return;

		if (mInstance->mLockDepth < 1)
			return;

		mInstance->mNewActors.Add(actor);
	}
}
