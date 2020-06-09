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
				data.AddMember("AssetId") = object->GetAssetID();
			else if (object->IsOnScene() || object->IsAsset())
				data.AddMember("ID") = object->GetID();
			else
			{
				auto dataNode = data.AddMember("Data");
				if (object)
					object->Serialize(dataNode);
				else
					dataNode = "null";
			}
		}
	}

	void ActorDataValueConverter::FromData(Actor*& object, const DataValue& data)
	{
		if (auto assetIdNode = data.FindMember("AssetId"))
		{
			UID assetId = *assetIdNode;
			object = o2Scene.GetAssetActorByID(assetId);

			if (!object)
				mUnresolvedActors.Add(ActorDef(&object, assetId));
		}
		else if (auto sceneIdNode = data.FindMember("ID"))
		{
			if (mLockDepth == 0)
				object = o2Scene.GetActorByID(*sceneIdNode);

			if (!object)
				mUnresolvedActors.Add(ActorDef(&object, (SceneUID)*sceneIdNode));
		}
		else if (auto dataValue = data.FindMember("Data"))
		{
			if ((String)dataValue == "null")
				object = nullptr;
			else
			{
				object = mnew Actor(ActorCreateMode::NotInScene);
				object->Deserialize(*dataValue);
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
			*def.target = mNewActors.FindMatchOrDefault([&](Actor* x) { return x->GetID() == def.actorId; });

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
