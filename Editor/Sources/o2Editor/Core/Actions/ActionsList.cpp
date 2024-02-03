#include "o2Editor/stdafx.h"
#include "ActionsList.h"

#include "o2Editor/Core/Actions/PropertyChange.h"
#include "o2Editor/SceneWindow/SceneEditScreen.h"

namespace Editor
{
	ActionsList::~ActionsList()
	{
		for (const auto& action : mActions)
			delete action;

		for (const auto& action : mForwardActions)
			delete action;
	}

	int ActionsList::GetUndoActionsCount() const
	{
		return mActions.Count();
	}

	int ActionsList::GetRedoActionsCount() const
	{
		return mForwardActions.Count();
	}

	String ActionsList::GetLastActionName() const
	{
		if (mActions.Count() > 0)
			return mActions.Last()->GetName();

		return "";
	}

	String ActionsList::GetNextForwardActionName() const
	{
		if (mForwardActions.Count() > 0)
			return mForwardActions.Last()->GetName();

		return "";
	}

	void ActionsList::UndoAction()
	{
		if (mActions.Count() > 0)
		{
			mActions.Last()->Undo();
			mForwardActions.Add(mActions.PopBack());
		}
	}

	void ActionsList::RedoAction()
	{
		if (mForwardActions.Count() > 0)
		{
			mForwardActions.Last()->Redo();
			mActions.Add(mForwardActions.PopBack());
		}
	}

	void ActionsList::DoneAction(const Ref<IAction>& action)
	{
		mActions.Add(action);

		for (const auto& action : mForwardActions)
			delete action;

		mForwardActions.Clear();
	}

	void ActionsList::DoneActorPropertyChangeAction(const String& path, const Vector<DataDocument>& prevValue,
													const Vector<DataDocument>& newValue)
	{
		Ref<PropertyChangeAction> action = mmake<PropertyChangeAction>(
			o2EditorSceneScreen.GetSelectedObjects(), path, prevValue, newValue);

		DoneAction(action);
	}

	void ActionsList::ResetUndoActions()
	{
		for (const auto& x : mActions)
			delete x;

		for (const auto& x : mForwardActions)
			delete x;

		mActions.Clear();
		mForwardActions.Clear();
	}

	const Vector<Ref<IAction>>& ActionsList::GetUndoActions() const
	{
		return mActions;
	}

	const Vector<Ref<IAction>>& ActionsList::GetRedoActions() const
	{
		return mForwardActions;
	}

}