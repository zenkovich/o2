#pragma once

#include "Core/Actions/IAction.h"
#include "Utils/Containers/Vector.h"

using namespace o2;

namespace o2
{
	class Actor;
}

namespace Editor
{
	class CreateActorsAction: public IAction
	{
	public:
		DataNode       actorsData;
		Vector<UInt64> actorsIds;
		UInt64         insertParentId;
		UInt64         insertPrevActorId;

	public:
		CreateActorsAction();
		CreateActorsAction(const Vector<Actor*>& actors, Actor* parent, Actor* prevActor);

		String GetName() const;
		void Redo();
		void Undo();

		SERIALIZABLE(CreateActorsAction);
	};

}

CLASS_BASES_META(Editor::CreateActorsAction)
{
	BASE_CLASS(Editor::IAction);
}
END_META;
CLASS_FIELDS_META(Editor::CreateActorsAction)
{
	PUBLIC_FIELD(actorsData);
	PUBLIC_FIELD(actorsIds);
	PUBLIC_FIELD(insertParentId);
	PUBLIC_FIELD(insertPrevActorId);
}
END_META;
CLASS_METHODS_META(Editor::CreateActorsAction)
{

	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(void, Redo);
	PUBLIC_FUNCTION(void, Undo);
}
END_META;
