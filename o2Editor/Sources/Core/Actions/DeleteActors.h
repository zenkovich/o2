#pragma once

#include "Core/Actions/IAction.h"
#include "Utils/Types/Containers/Vector.h"
#include "Utils/Serialization/DataNode.h"

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
			DataNode actorData;	  // @SERIALIZABLE
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

CLASS_BASES_META(Editor::DeleteActorsAction)
{
	BASE_CLASS(Editor::IAction);
}
END_META;
CLASS_FIELDS_META(Editor::DeleteActorsAction)
{
	PUBLIC_FIELD(actorsInfos);
}
END_META;
CLASS_METHODS_META(Editor::DeleteActorsAction)
{

	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(void, Redo);
	PUBLIC_FUNCTION(void, Undo);
	PROTECTED_FUNCTION(int, GetActorIdx, Actor*);
}
END_META;

CLASS_BASES_META(Editor::DeleteActorsAction::ActorInfo)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(Editor::DeleteActorsAction::ActorInfo)
{
	PUBLIC_FIELD(actorData).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(parentId).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(prevActorId).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(idx).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(Editor::DeleteActorsAction::ActorInfo)
{
}
END_META;
