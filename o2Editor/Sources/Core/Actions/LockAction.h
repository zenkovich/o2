#pragma once

#include "Core/Actions/IAction.h"
#include "Utils/Containers/Vector.h"

using namespace o2;

namespace o2
{
	class Actor;
}

class EditorLockAction: public IEditorAction
{
public:
	Vector<UInt64> actorsIds;
	bool           lock;

public:
	EditorLockAction();
	EditorLockAction(const Vector<Actor*>& actors, bool lock);

	String GetName() const;
	void Redo();
	void Undo();

	SERIALIZABLE(EditorLockAction);
};
