#pragma once

#include "o2Editor/Core/Actions/IAction.h"

namespace Editor
{
	class ActionsList
	{
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
		void DoneActorPropertyChangeAction(const String& path, const Vector<DataValue>& prevValue, const Vector<DataValue>& newValue);

		// Resets undo and redo actions
		void ResetUndoActions();

		// Returns done actions
		const Vector<IAction*> GetUndoActions() const;

		// Returns redo actions
		const Vector<IAction*> GetRedoActions() const;

	protected:
		Vector<IAction*> mActions;        // Done actions
		Vector<IAction*> mForwardActions; // Forward actions, what you can redo
	};
}
