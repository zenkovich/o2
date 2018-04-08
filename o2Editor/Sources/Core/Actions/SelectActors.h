#pragma once

#include "Core/Actions/IAction.h"
#include "Utils/Types/Containers/Vector.h"

using namespace o2;

namespace o2
{
	class Actor;
}

namespace Editor
{
	class SelectActorsAction: public IAction
	{
	public:
		Vector<UInt64> selectedActorsIds;
		Vector<UInt64> prevSelectedActorsIds;

	public:
		SelectActorsAction();
		SelectActorsAction(const Vector<Actor*>& selectedActors, const Vector<Actor*>& prevSelectedActors);
		~SelectActorsAction();

		String GetName() const;
		void Redo();
		void Undo();

		SERIALIZABLE(SelectActorsAction);
	};
}

CLASS_BASES_META(Editor::SelectActorsAction)
{
	BASE_CLASS(Editor::IAction);
}
END_META;
CLASS_FIELDS_META(Editor::SelectActorsAction)
{
	PUBLIC_FIELD(selectedActorsIds);
	PUBLIC_FIELD(prevSelectedActorsIds);
}
END_META;
CLASS_METHODS_META(Editor::SelectActorsAction)
{

	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(void, Redo);
	PUBLIC_FUNCTION(void, Undo);
}
END_META;
