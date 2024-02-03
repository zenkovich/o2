#pragma once

#include "o2Editor/Core/Actions/IAction.h"
#include <memory>
#include <vector>

namespace Editor
{
	template<class T>
	using Ref = std::shared_ptr<T>;

	template<class T>
	using WeakRef = std::weak_ptr<T>;

	template<class T, class... Args>
	constexpr Ref<T> mmake(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<class T, class U>
	constexpr Ref<T> DynamicCast(const Ref<U>& r)
	{
		return std::dynamic_pointer_cast<T>(r);
	}

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

		// Called when action was done
		void DoneAction(const Ref<IAction>& action);

		// Called when some property changed, stores action for undo
		void DoneActorPropertyChangeAction(const String& path,
										   const Vector<DataDocument>& prevValue, const Vector<DataDocument>& newValue);

		// Resets undo and redo actions
		void ResetUndoActions();

		// Returns done actions
		const Vector<Ref<IAction>>& GetUndoActions() const;

		// Returns redo actions
		const Vector<Ref<IAction>>& GetRedoActions() const;

	protected:
		Vector<Ref<IAction>> mActions;        // Done actions
		Vector<Ref<IAction>> mForwardActions; // Forward actions, what you can redo
	};
}