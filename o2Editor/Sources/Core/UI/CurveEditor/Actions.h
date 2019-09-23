#pragma once

#include "Core/Actions/IAction.h"
#include "Core/UI/CurveEditor/CurveEditor.h"

namespace Editor
{
	class AddKeysAction : public IAction
	{
	public:
		AddKeysAction();
		AddKeysAction(const CurveEditor::CurveKeysInfosVec& infos, CurveEditor* editor);

		String GetName();
		void Redo();
		void Undo();

		SERIALIZABLE(AddKeysAction);

	protected:
		CurveEditor::CurveKeysInfosVec mInfos;
		CurveEditor*                   mEditor;
	};

	class DeleteKeysAction : public IAction
	{
	public:
		DeleteKeysAction();
		DeleteKeysAction(const CurveEditor::CurveKeysInfosVec& infos, CurveEditor* editor);

		String GetName();
		void Redo();
		void Undo();

		SERIALIZABLE(DeleteKeysAction);

	protected:
		CurveEditor::CurveKeysInfosVec mInfos;
		CurveEditor*                   mEditor;
	};

	class KeysChangeAction : public IAction
	{
	public:
		struct KeysInfo
		{
			String                               curveId;
			Curve::KeysVec                       beforeKeys;
			Curve::KeysVec                       afterKeys;
			CurveEditor::SelectedHandlesInfosVec selectedHandles;

			bool operator==(const KeysInfo& other) const;
		};
		typedef Vector<KeysInfo> KeysInfosVec;

	public:
		KeysChangeAction();
		KeysChangeAction(const KeysInfosVec& infos, CurveEditor* editor);

		String GetName();
		void Redo();
		void Undo();

		SERIALIZABLE(KeysChangeAction);

	protected:
		KeysInfosVec   mInfos;
		CurveEditor*   mEditor;
	};
}

CLASS_BASES_META(Editor::AddKeysAction)
{
	BASE_CLASS(Editor::IAction);
}
END_META;
CLASS_FIELDS_META(Editor::AddKeysAction)
{
	PROTECTED_FIELD(mInfos);
	PROTECTED_FIELD(mEditor);
}
END_META;
CLASS_METHODS_META(Editor::AddKeysAction)
{

	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(void, Redo);
	PUBLIC_FUNCTION(void, Undo);
}
END_META;

CLASS_BASES_META(Editor::DeleteKeysAction)
{
	BASE_CLASS(Editor::IAction);
}
END_META;
CLASS_FIELDS_META(Editor::DeleteKeysAction)
{
	PROTECTED_FIELD(mInfos);
	PROTECTED_FIELD(mEditor);
}
END_META;
CLASS_METHODS_META(Editor::DeleteKeysAction)
{

	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(void, Redo);
	PUBLIC_FUNCTION(void, Undo);
}
END_META;

CLASS_BASES_META(Editor::KeysChangeAction)
{
	BASE_CLASS(Editor::IAction);
}
END_META;
CLASS_FIELDS_META(Editor::KeysChangeAction)
{
	PROTECTED_FIELD(mInfos);
	PROTECTED_FIELD(mEditor);
}
END_META;
CLASS_METHODS_META(Editor::KeysChangeAction)
{

	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(void, Redo);
	PUBLIC_FUNCTION(void, Undo);
}
END_META;
