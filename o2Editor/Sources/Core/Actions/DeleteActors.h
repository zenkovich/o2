#pragma once

#include "Core/Actions/IAction.h"
#include "Utils/Containers/Vector.h"
#include "Utils/Data/DataDoc.h"

using namespace o2;

namespace o2
{
	class Actor;
}

namespace Editor
{
	class DeleteActorsAction: public IAction
	{
	public:
		class ActorInfo: public ISerializable
		{
		public:
			DataNode actor;		  // @SERIALIZABLE
			UInt64   parentId;	  // @SERIALIZABLE
			UInt64   prevActorId; // @SERIALIZABLE
			int      idx;         // @SERIALIZABLE

			bool operator==(const ActorInfo& other) const;

			SERIALIZABLE(ActorInfo);
		};

	public:
		Vector<ActorInfo> actorsInfos;

	public:
		DeleteActorsAction();
		DeleteActorsAction(const Vector<Actor*>& deletingActors);
		~DeleteActorsAction();

		String GetName() const;
		void Redo();
		void Undo();

		SERIALIZABLE(DeleteActorsAction);

	protected:
		int GetActorIdx(Actor* actor);
	};
}