#include "stdafx.h"
#include "ActorDataNodeConverter.h"

#include "Scene/Actor.h"
#include "Scene/Scene.h"

namespace o2
{
	DECLARE_SINGLETON(ActorDataNodeConverter);

// 	void ActorDataNodeConverter::ToData(const void* object, DataNode& data)
// 	{
// 		const Actor* value = *(const Actor**)object;
// 
// 		if (value)
// 		{
// 			if (value->mIsAsset)
// 				*data.AddNode("AssetId") = value->GetAssetID();
// 			else if (value->IsOnScene() || value->IsAsset())
// 				*data.AddNode("ID") = value->GetID();
// 			else
// 				*data.AddNode("Data") = value ? value->Serialize() : (String)"null";
// 		}
// 	}
// 
// 	void ActorDataNodeConverter::FromData(void* object, const DataNode& data)
// 	{
// 		Actor*& actor = *(Actor**)object;
// 
// 		if (auto assetIdNode = data.GetNode("AssetId"))
// 		{
// 			UID assetId = *assetIdNode;
// 			actor = o2Scene.GetAssetActorByID(assetId);
// 
// 			if (!actor)
// 				mUnresolvedActors.Add(ActorDef(&actor, assetId));
// 		}
// 		else if (auto sceneIdNode = data.GetNode("ID"))
// 		{
// 			if (mLockDepth == 0)
// 				actor = o2Scene.GetActorByID(*sceneIdNode);
// 
// 			if (!actor)
// 				mUnresolvedActors.Add(ActorDef(&actor, (SceneUID)*sceneIdNode));
// 		}
// 		else if (auto dataNode = data.GetNode("Data"))
// 		{
// 			if (dataNode->Data() == "null")
// 				actor = nullptr;
// 			else
// 			{
// 				actor = mnew Actor(ActorCreateMode::NotInScene);
// 				actor->Deserialize(*dataNode);
// 			}
// 		}
// 		else actor = nullptr;
// 	}
// 
// 	bool ActorDataNodeConverter::IsConvertsType(const Type* type) const
// 	{
// 		return type->IsBasedOn(*TypeOf(Actor).GetPointerType());
// 	}
// 
// 	void ActorDataNodeConverter::LockPointersResolving()
// 	{
// 		mLockDepth++;
// 	}
// 
// 	void ActorDataNodeConverter::UnlockPointersResolving()
// 	{
// 		mLockDepth--;
// 
// 		if (mLockDepth < 0)
// 		{
// 			o2Debug.LogWarning("ActorDataNodeConverter lock depth is less than zero. Something going wrong");
// 			mLockDepth = 0;
// 		}
// 	}
// 
// 	void ActorDataNodeConverter::ResolvePointers()
// 	{
// 		if (mLockDepth > 0)
// 			return;
// 
// 		for (auto def : mUnresolvedActors)
// 		{
// 			*def.target = mNewActors.FindMatch([&](Actor* x) { return x->GetID() == def.actorId; });
// 
// 			if (!*def.target)
// 			{
// 				if (def.isAsset)
// 					*def.target = o2Scene.GetAssetActorByID(def.assetId);
// 				else
// 					*def.target = o2Scene.GetActorByID(def.actorId);
// 			}
// 		}
// 
// 		mNewActors.Clear();
// 		mUnresolvedActors.Clear();
// 	}
// 
// 	void ActorDataNodeConverter::ActorCreated(Actor* actor)
// 	{
// 		if (!IsSingletonInitialzed())
// 			return;
// 
// 		if (mInstance->mLockDepth < 1)
// 			return;
// 
// 		mInstance->mNewActors.Add(actor);
// 	}
}