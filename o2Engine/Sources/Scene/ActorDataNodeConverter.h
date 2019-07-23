#pragma once

#include "Utils/Serialization/DataNode.h"
#include "Utils/Singleton.h"

namespace o2
{
	// -------------------------
	// Actor data node converter
	// -------------------------
// 	class ActorDataNodeConverter : public IDataNodeTypeConverter, public Singleton<ActorDataNodeConverter>
// 	{
// 	public:
// 		// Converts actor pointer to data 
// 		void ToData(const void* object, DataNode& data);
// 
// 		// Gets actor pointer from data
// 		void FromData(void* object, const DataNode& data);
// 
// 		// Checks that type is based on Actor's type
// 		bool IsConvertsType(const Type* type) const;
// 
// 	protected:
// 		struct ActorDef
// 		{
// 			Actor** target;
// 			bool    isAsset;
// 			UInt64  actorId;
// 			UID     assetId;
// 
// 		public:
// 			ActorDef() {}
// 
// 			ActorDef(Actor** target, UInt64 id) :
// 				target(target), isAsset(false), actorId(id)
// 			{}
// 
// 			ActorDef(Actor** target, UID id) :
// 				target(target), isAsset(true), assetId(id)
// 			{}
// 
// 			bool operator==(const ActorDef& other) const
// 			{
// 				return target == other.target;
// 			}
// 		};
// 
// 		typedef Vector<ActorDef> ActorDefsVec;
// 		typedef Vector<Actor*> ActorsVec;
// 
// 	protected:
// 		ActorDefsVec mUnresolvedActors;
// 		ActorsVec    mNewActors;
// 		int          mLockDepth = 0;
// 
// 	protected:
// 		// Locks pointers resolving depth
// 		void LockPointersResolving();
// 
// 		// Unlocks pointers resolving depth
// 		void UnlockPointersResolving();
// 
// 		// Resolves actors' pointers. Works when lock depth is 0
// 		void ResolvePointers();
// 
// 		// It is called when new actor was created
// 		static void ActorCreated(Actor* actor);
// 
// 		friend class Actor;
// 		friend class Scene;
// 	};
}
