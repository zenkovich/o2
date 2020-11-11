#pragma once

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
		static void RequireResolve(ComponentRef& ref, SceneUID id);

		// Locks references resolving depth
		static void LockResolving();

		// Unlocks references resolving depth
		static void UnlockResolving();

		// Resolves references. Works when lock depth is 0
		static void ResolveRefs();

		// Returns is resolver depth locked
		static bool IsLocked();

		// It is called when new actor was created
		static void ActorCreated(Actor* actor);

		// It is called when new component was created
		static void ComponentCreated(Component* component);

	protected:
		struct UnresolvedActorRef
		{
			ActorRef* target;
			bool      isSourceAsset;
			SceneUID  sourceActorId;
			UID       sourceAssetId;

		public:
			UnresolvedActorRef();
			UnresolvedActorRef(ActorRef* target, SceneUID actorId);
			UnresolvedActorRef(ActorRef* target, const UID& assetId);

			bool operator==(const UnresolvedActorRef& other) const;
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
		Vector<UnresolvedActorRef> mUnresolvedActors;
		Map<SceneUID, Actor*>      mNewActors;

		Vector<UnresolvedComponentRef> mUnresolvedComponents;
		Map<SceneUID, Component*>      mNewComponents;

		int mLockDepth = 0;

		friend class Actor;
		friend class Component;
		friend class Scene;
	};

	template<>
	struct DataValue::Converter<ActorRef>
	{
		static constexpr bool isSupported = true;

		static void Write(const ActorRef& value, DataValue& data)
		{
			if (value)
			{
				if (value->mIsAsset)
					data.AddMember("AssetId") = value->GetAssetID();
				else if (value->IsOnScene())
					data.AddMember("ID") = value->GetID();
				else
				{
					auto dataNode = data.AddMember("Data");
					value->Serialize(dataNode);
				}
			}
		}

		static void Read(ActorRef& value, const DataValue& data)
		{
			if (auto assetIdNode = data.FindMember("AssetId"))
			{
				UID assetId = *assetIdNode;
				value = o2Scene.GetAssetActorByID(assetId);

				if (!value)
					ActorRefResolver::RequireResolve(value, assetId);
			}
			else if (auto sceneIdNode = data.FindMember("ID"))
			{
				if (!ActorRefResolver::IsLocked())
					value = o2Scene.GetActorByID(*sceneIdNode);

				if (!value)
					ActorRefResolver::RequireResolve(value, (SceneUID)*sceneIdNode);
			}
			else if (auto dataValue = data.FindMember("Data"))
			{
				if ((String)dataValue == "null")
					value = nullptr;
				else
				{
					value = mnew Actor(ActorCreateMode::NotInScene);
					value->Deserialize(*dataValue);
				}
			}
			else value = nullptr;
		}
	};
}
