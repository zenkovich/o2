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
	class LockAction: public IAction
	{
	public:
		Vector<UInt64> actorsIds;
		bool           lock;

	public:
		LockAction();
		LockAction(const Vector<Actor*>& actors, bool lock);

		String GetName() const;
		void Redo();
		void Undo();

		SERIALIZABLE(LockAction);
	};
}