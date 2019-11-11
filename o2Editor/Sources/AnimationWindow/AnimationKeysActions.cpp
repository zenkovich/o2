#include "stdafx.h"
#include "AnimationKeysActions.h"

namespace Editor
{
	AnimationAddKeysAction::AnimationAddKeysAction()
	{}

	AnimationAddKeysAction::AnimationAddKeysAction(const DataNode& keysData, KeyHandlesSheet* editor):
		mKeysData(keysData), mEditor(editor)
	{}

	String AnimationAddKeysAction::GetName()
	{
		return "Add keys";
	}

	void AnimationAddKeysAction::Redo()
	{
		Dictionary<String, Vector<UInt64>> keys;
		mEditor->DeserializeKeys(mKeysData, keys, 0.0f);
	}

	void AnimationAddKeysAction::Undo()
	{

	}

	AnimationDeleteKeysAction::AnimationDeleteKeysAction()
	{}

	AnimationDeleteKeysAction::AnimationDeleteKeysAction(const DataNode& keysData, KeyHandlesSheet* editor) :
		mKeysData(keysData), mEditor(editor)
	{}

	String AnimationDeleteKeysAction::GetName()
	{
		return "Delete keys";
	}

	void AnimationDeleteKeysAction::Redo()
	{

	}

	void AnimationDeleteKeysAction::Undo()
	{

	}

	AnimationKeysChangeAction::AnimationKeysChangeAction()
	{}

	AnimationKeysChangeAction::AnimationKeysChangeAction(const DataNode& beforeKeysData, 
														 const DataNode& afterKeysData, 
														 KeyHandlesSheet* editor):
		mBeforeKeysData(beforeKeysData), mAfterKeysData(afterKeysData), mEditor(editor)
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
