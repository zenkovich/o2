#pragma once

#include "Core/Actions/IAction.h"
#include "Utils/Containers/Vector.h"

using namespace o2;

namespace o2
{
	class Actor;
}

class EditorSelectionAction: public IEditorAction
{
public:
	Vector<UInt64> selectedActorsIds;
	Vector<UInt64> prevSelectedActorsIds;

public:
	EditorSelectionAction();
	EditorSelectionAction(const Vector<Actor*>& selectedActors, const Vector<Actor*>& prevSelectedActors);
	~EditorSelectionAction();

	String GetName() const;
	void Redo();
	void Undo();

	SERIALIZABLE(EditorSelectionAction);
};
