#pragma once

#include "o2/Utils/Serialization/DataValue.h"
#include "o2/Utils/Singleton.h"

namespace o2
{
	// -------------------------
	// Actor data node converter
	// -------------------------
	class ActorDataValueConverter : public Singleton<ActorDataValueConverter>
	{
	public:
		// Converts actor pointer to data 
		void ToData(const Actor* object, DataValue& data);

		// Gets actor pointer from data
		void FromData(Actor*& object, const DataValue& data);

	protected:
		struct ActorDef
		{
			Actor** target;
			bool    isAsset;
			UInt64  actorId;
			UID     assetId;

		public:
			ActorDef() {}

			ActorDef(Actor** target, UInt64 id) :
				target(target), isAsset(false), actorId(id)
			{}

			ActorDef(Actor** target, const UID& id) :
				target(target), isAsset(true), assetId(id)
			{}

			bool operator==(const ActorDef& other) const
			{
				return target == other.target;
			}
		};

	protected:
		Vector<ActorDef> mUnresolvedActors;
		Vector<Actor*>   mNewActors;

		int mLockDepth = 0;

	protected:
		// Locks pointers resolving depth
		void LockPointersResolving();

		// Unlocks pointers resolving depth
		void UnlockPointersResolving();

		// Resolves actors' pointers. Works when lock depth is 0
		void ResolvePointers();

		// It is called when new actor was created
		static void ActorCreated(Actor* actor);

		friend class Actor;
		friend class Scene;
	};

	template<>
	struct DataValue::Converter<Actor*>
	{
		static constexpr bool isSupported = true;

		using ActorPtr = Actor *;

		static void Write(const ActorPtr& value, DataValue& data)
		{
			ActorDataValueConverter::Instance().ToData(value, data);
		}

		static void Read(ActorPtr& value, const DataValue& data)
		{
			ActorDataValueConverter::Instance().FromData(value, data);
		}
	};
}
