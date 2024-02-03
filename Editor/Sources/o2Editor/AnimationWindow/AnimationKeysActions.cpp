#include "o2Editor/stdafx.h"
#include "AnimationKeysActions.h"
#include "Ref.h"
#include "WeakRef.h"

namespace Editor
{
	AnimationAddKeysAction::AnimationAddKeysAction()
	{}

	AnimationAddKeysAction::AnimationAddKeysAction(const Map<String, Vector<UInt64>>& keys, DataDocument& keysData, KeyHandlesSheet* editor):
		mKeys(keys), mKeysData(keysData), mEditor(editor)
	{}

	String AnimationAddKeysAction::GetName() const
	{
		return "Add keys";
	}

	void AnimationAddKeysAction::Redo()
	{
		Map<String, Vector<UInt64>> keys;
		mEditor->DeserializeKeys(mKeysData, keys, 0.0f, false);
		mEditor->SetSelectedKeys(keys);
	}

	void AnimationAddKeysAction::Undo()
	{
		mEditor->DeleteKeys(mKeys, false);
	}

	AnimationDeleteKeysAction::AnimationDeleteKeysAction()
	{}

	AnimationDeleteKeysAction::AnimationDeleteKeysAction(const Map<String, Vector<UInt64>>& keys, DataDocument& keysData, KeyHandlesSheet* editor) :
		mKeys(keys), mKeysData(keysData), mEditor(editor)
	{}

	String AnimationDeleteKeysAction::GetName() const
	{
		return "Delete keys";
	}

	void AnimationDeleteKeysAction::Redo()
	{
		mEditor->DeleteKeys(mKeys, false);
	}

	void AnimationDeleteKeysAction::Undo()
	{
		Map<String, Vector<UInt64>> keys;
		mEditor->DeserializeKeys(mKeysData, keys, 0.0f, false);
		mEditor->SetSelectedKeys(keys);
	}

	AnimationKeysChangeAction::AnimationKeysChangeAction()
	{}

	AnimationKeysChangeAction::AnimationKeysChangeAction(const Map<String, Vector<UInt64>>& keys, DataDocument& beforeKeysData,
														 DataDocument& afterKeysData, KeyHandlesSheet* editor):
		mKeys(keys), mBeforeKeysData(beforeKeysData), mAfterKeysData(afterKeysData), mEditor(editor)
	{}

	String AnimationKeysChangeAction::GetName() const
	{
		return "Changed keys";
	}

	void AnimationKeysChangeAction::Redo()
	{
		mEditor->DeleteKeys(mKeys, false);

		Map<String, Vector<UInt64>> keys;
		mEditor->DeserializeKeys(mAfterKeysData, keys, 0.0f, false);
		mEditor->SetSelectedKeys(keys);
	}

	void AnimationKeysChangeAction::Undo()
	{
		mEditor->DeleteKeys(mKeys, false);

		Map<String, Vector<UInt64>> keys;
		mEditor->DeserializeKeys(mBeforeKeysData, keys, 0.0f, false);
		mEditor->SetSelectedKeys(keys);
	}
}
// --- META ---

DECLARE_CLASS(Editor::AnimationAddKeysAction, Editor__AnimationAddKeysAction);

DECLARE_CLASS(Editor::AnimationDeleteKeysAction, Editor__AnimationDeleteKeysAction);

DECLARE_CLASS(Editor::AnimationKeysChangeAction, Editor__AnimationKeysChangeAction);
// --- END META ---