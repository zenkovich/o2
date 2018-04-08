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
	class LockActorsAction: public IAction
	{
	public:
		Vector<UInt64> actorsIds;
		bool           lock;

	public:
		LockActorsAction();
		LockActorsAction(const Vector<Actor*>& actors, bool lock);

		String GetName() const;
		void Redo();
		void Undo();

		SERIALIZABLE(LockActorsAction);
	};
}

CLASS_BASES_META(Editor::LockActorsAction)
{
	BASE_CLASS(Editor::IAction);
}
END_META;
CLASS_FIELDS_META(Editor::LockActorsAction)
{
	PUBLIC_FIELD(actorsIds);
	PUBLIC_FIELD(lock);
}
END_META;
CLASS_METHODS_META(Editor::LockActorsAction)
{

	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(void, Redo);
	PUBLIC_FUNCTION(void, Undo);
}
END_META;
