#pragma once

#include "Core/Actions/IAction.h"

namespace Editor
{
	class ActionsList
	{
	public:
		typedef Vector<IAction*> EditorActionsVec;

	public:
		// Destructor. Destroys stored actions
		~ActionsList();

		// Returns count of undo actions
		int GetUndoActionsCount() const;

		// Returns count of redo actions
		int GetRedoActionsCount() const;

		// Returns last action name
		String GetLastActionName() const;

		// Returns next forward action name
		String GetNextForwardActionName() const;

		// Undo last action
		void UndoAction();

		// Redo next action
		void RedoAction();

		// It is called when action was done
		void DoneAction(IAction* action);

		// It is called when some property changed, stores action for undo
		void DoneActorPropertyChangeAction(const String& path, const Vector<DataNode>& prevValue, const Vector<DataNode>& newValue);

		// Resets undo and redo actions
		void ResetUndoActions();

	protected:
		EditorActionsVec mActions;        // Done actions
		EditorActionsVec mForwardActions; // Forward actions, what you can redo
	};
}
