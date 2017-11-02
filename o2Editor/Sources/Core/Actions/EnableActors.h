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
	class EnableActorsAction: public IAction
	{
	public:
		Vector<UInt64> actorsIds;
		bool           enable;

	public:
		EnableActorsAction();
		EnableActorsAction(const Vector<Actor*>& actors, bool enable);

		String GetName() const;
		void Redo();
		void Undo();

		SERIALIZABLE(EnableActorsAction);
	};
}

CLASS_BASES_META(Editor::EnableActorsAction)
{
	BASE_CLASS(Editor::IAction);
}
END_META;
CLASS_FIELDS_META(Editor::EnableActorsAction)
{
	PUBLIC_FIELD(actorsIds);
	PUBLIC_FIELD(enable);
}
END_META;
CLASS_METHODS_META(Editor::EnableActorsAction)
{

	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(void, Redo);
	PUBLIC_FUNCTION(void, Undo);
}
END_META;
