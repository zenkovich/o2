#pragma once

#include "Core/Actions/IAction.h"
#include "Utils/Types/Containers/Vector.h"
#include "Utils/Math/Basis.h"

using namespace o2;

namespace o2
{
	class Actor;
}

namespace Editor
{
	class ReparentActorsAction: public IAction
	{
	public:
		struct ActorInfo
		{
			UInt64 actorId;
			UInt64 lastParentId;
			UInt64 lastPrevActorId;
			int    actorHierarchyIdx;
			Basis  transform;
		};

		Vector<ActorInfo*> actorsInfos;
		UInt64             newParentId;
		UInt64             newPrevActorId;

	public:
		ReparentActorsAction();
		ReparentActorsAction(const Vector<Actor*>& beginActors);
		~ReparentActorsAction();

		void ActorsReparented(Actor* newParent, Actor* prevActor);

		String GetName() const;
		void Redo();
		void Undo();

		SERIALIZABLE(ReparentActorsAction);
	};
}

CLASS_BASES_META(Editor::ReparentActorsAction)
{
	BASE_CLASS(Editor::IAction);
}
END_META;
CLASS_FIELDS_META(Editor::ReparentActorsAction)
{
	PUBLIC_FIELD(actorsInfos);
	PUBLIC_FIELD(newParentId);
	PUBLIC_FIELD(newPrevActorId);
}
END_META;
CLASS_METHODS_META(Editor::ReparentActorsAction)
{

	PUBLIC_FUNCTION(void, ActorsReparented, Actor*, Actor*);
	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(void, Redo);
	PUBLIC_FUNCTION(void, Undo);
}
END_META;
