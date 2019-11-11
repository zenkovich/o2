#include "stdafx.h"
#include "Actions.h"

namespace Editor
{
	AnimationAddKeysAction::AnimationAddKeysAction()
	{}

	AnimationAddKeysAction::AnimationAddKeysAction(const Vector<AnimationKeysData>& keysData, KeyHandlesSheet* editor):
		mKeysData(keysData), mEditor(editor)
	{}

	String AnimationAddKeysAction::GetName()
	{
		return "Add keys";
	}

	void AnimationAddKeysAction::Redo()
	{

	}

	void AnimationAddKeysAction::Undo()
	{

	}

	AnimationDeleteKeysAction::AnimationDeleteKeysAction()
	{}

	AnimationDeleteKeysAction::AnimationDeleteKeysAction(const Vector<AnimationKeysData>& keysData, KeyHandlesSheet* editor) :
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

	AnimationKeysChangeAction::AnimationKeysChangeAction(const Vector<AnimationKeysData>& beforeKeysData, 
														 const Vector<AnimationKeysData>& afterKeysData, 
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
