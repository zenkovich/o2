#include "stdafx.h"
#include "AnimationKeysActions.h"

namespace Editor
{
	AnimationAddKeysAction::AnimationAddKeysAction()
	{}

	AnimationAddKeysAction::AnimationAddKeysAction(const Map<String, Vector<UInt64>>& keys, const DataNode& keysData, KeyHandlesSheet* editor):
		mKeys(keys), mKeysData(keysData), mEditor(editor)
	{}

	String AnimationAddKeysAction::GetName()
	{
		return "Add keys";
	}

	void AnimationAddKeysAction::Redo()
	{
		Map<String, Vector<UInt64>> keys;
		mEditor->DeserializeKeys(mKeysData, keys, 0.0f);
		mEditor->SetSelectedKeys(keys);
	}

	void AnimationAddKeysAction::Undo()
	{
		mEditor->DeleteKeys(mKeys);
	}

	AnimationDeleteKeysAction::AnimationDeleteKeysAction()
	{}

	AnimationDeleteKeysAction::AnimationDeleteKeysAction(const Map<String, Vector<UInt64>>& keys, const DataNode& keysData, KeyHandlesSheet* editor) :
		mKeys(keys), mKeysData(keysData), mEditor(editor)
	{}

	String AnimationDeleteKeysAction::GetName()
	{
		return "Delete keys";
	}

	void AnimationDeleteKeysAction::Redo()
	{
		mEditor->DeleteKeys(mKeys);
	}

	void AnimationDeleteKeysAction::Undo()
	{
		Map<String, Vector<UInt64>> keys;
		mEditor->DeserializeKeys(mKeysData, keys, 0.0f);
		mEditor->SetSelectedKeys(keys);
	}

	AnimationKeysChangeAction::AnimationKeysChangeAction()
	{}

	AnimationKeysChangeAction::AnimationKeysChangeAction(const Map<String, Vector<UInt64>>& keys, const DataNode& beforeKeysData,
														 const DataNode& afterKeysData, KeyHandlesSheet* editor):
		mKeys(keys), mBeforeKeysData(beforeKeysData), mAfterKeysData(afterKeysData), mEditor(editor)
	{}

	String AnimationKeysChangeAction::GetName()
	{
		return "Changed keys";
	}

	void AnimationKeysChangeAction::Redo()
	{

	}

	void AnimationKeysChangeAction::Undo()
	{

	}
}

DECLARE_CLASS(Editor::AnimationAddKeysAction);

DECLARE_CLASS(Editor::AnimationDeleteKeysAction);

DECLARE_CLASS(Editor::AnimationKeysChangeAction);
