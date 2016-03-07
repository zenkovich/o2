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
	class SelectionAction: public IAction
	{
	public:
		Vector<UInt64> selectedActorsIds;
		Vector<UInt64> prevSelectedActorsIds;

	public:
		SelectionAction();
		SelectionAction(const Vector<Actor*>& selectedActors, const Vector<Actor*>& prevSelectedActors);
		~SelectionAction();

		String GetName() const;
		void Redo();
		void Undo();

		SERIALIZABLE(SelectionAction);
	};
}