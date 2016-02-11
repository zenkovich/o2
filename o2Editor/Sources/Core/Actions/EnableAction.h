#pragma once

#include "Core/Actions/IAction.h"
#include "Utils/Containers/Vector.h"

using namespace o2;

namespace o2
{
	class Actor;
}

class EditorEnableAction: public IEditorAction
{
public:
	Vector<UInt64> actorsIds;
	bool           enable;

public:
	EditorEnableAction();
	EditorEnableAction(const Vector<Actor*>& actors, bool enable);

	String GetName() const;
	void Redo();
	void Undo();

	SERIALIZABLE(EditorEnableAction);
};
