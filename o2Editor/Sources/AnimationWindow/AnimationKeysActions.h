#pragma once

#include "Core/Actions/IAction.h"
#include "KeyHandlesSheet.h"

namespace Editor
{
	class AnimationAddKeysAction : public IAction
	{
	public:
		AnimationAddKeysAction();
		AnimationAddKeysAction(const Map<String, Vector<UInt64>>& keys, const DataNode& keysData, KeyHandlesSheet* editor);

		String GetName();
		void Redo();
		void Undo();

		SERIALIZABLE(AnimationAddKeysAction);

	protected:
		Map<String, Vector<UInt64>> mKeys;
		DataNode                    mKeysData;
		KeyHandlesSheet*            mEditor;
	};

	class AnimationDeleteKeysAction : public IAction
	{
	public:
		AnimationDeleteKeysAction();
		AnimationDeleteKeysAction(const Map<String, Vector<UInt64>>& keys, const DataNode& keysData, KeyHandlesSheet* editor);

		String GetName();
		void Redo();
		void Undo();

		SERIALIZABLE(AnimationDeleteKeysAction);

	protected:
		Map<String, Vector<UInt64>> mKeys;
		DataNode                    mKeysData;
		KeyHandlesSheet*            mEditor;
	};

	class AnimationKeysChangeAction : public IAction
	{
	public:
		AnimationKeysChangeAction();
		AnimationKeysChangeAction(const Map<String, Vector<UInt64>>& keys,  const DataNode& beforeKeysData,
								  const DataNode& afterKeysData, KeyHandlesSheet* editor);

		String GetName();
		void Redo();
		void Undo();

		SERIALIZABLE(AnimationKeysChangeAction);

	protected:
		Map<String, Vector<UInt64>> mKeys;
		DataNode                    mBeforeKeysData;
		DataNode                    mAfterKeysData;
		KeyHandlesSheet*            mEditor;
	};
}

CLASS_BASES_META(Editor::AnimationAddKeysAction)
{
	BASE_CLASS(Editor::IAction);
}
END_META;
CLASS_FIELDS_META(Editor::AnimationAddKeysAction)
{
	PROTECTED_FIELD(mKeysData);
	PROTECTED_FIELD(mEditor);
}
END_META;
CLASS_METHODS_META(Editor::AnimationAddKeysAction)
{

	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(void, Redo);
	PUBLIC_FUNCTION(void, Undo);
}
END_META;

CLASS_BASES_META(Editor::AnimationDeleteKeysAction)
{
	BASE_CLASS(Editor::IAction);
}
END_META;
CLASS_FIELDS_META(Editor::AnimationDeleteKeysAction)
{
	PROTECTED_FIELD(mKeysData);
	PROTECTED_FIELD(mEditor);
}
END_META;
CLASS_METHODS_META(Editor::AnimationDeleteKeysAction)
{

	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(void, Redo);
	PUBLIC_FUNCTION(void, Undo);
}
END_META;

CLASS_BASES_META(Editor::AnimationKeysChangeAction)
{
	BASE_CLASS(Editor::IAction);
}
END_META;
CLASS_FIELDS_META(Editor::AnimationKeysChangeAction)
{
	PROTECTED_FIELD(mBeforeKeysData);
	PROTECTED_FIELD(mAfterKeysData);
	PROTECTED_FIELD(mEditor);
}
END_META;
CLASS_METHODS_META(Editor::AnimationKeysChangeAction)
{

	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(void, Redo);
	PUBLIC_FUNCTION(void, Undo);
}
END_META;
