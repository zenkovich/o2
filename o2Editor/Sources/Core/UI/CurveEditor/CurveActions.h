#pragma once

#include "Core/Actions/IAction.h"
#include "Core/UI/CurveEditor/CurveEditor.h"

namespace Editor
{
	class CurveAddKeysAction : public IAction
	{
	public:
		CurveAddKeysAction();
		CurveAddKeysAction(const CurveEditor::CurveKeysInfosVec& infos, CurveEditor* editor);

		String GetName();
		void Redo();
		void Undo();

		SERIALIZABLE(CurveAddKeysAction);

	protected:
		CurveEditor::CurveKeysInfosVec mInfos;
		CurveEditor*                   mEditor;
	};

	class CurveDeleteKeysAction : public IAction
	{
	public:
		CurveDeleteKeysAction();
		CurveDeleteKeysAction(const CurveEditor::CurveKeysInfosVec& infos, CurveEditor* editor);

		String GetName();
		void Redo();
		void Undo();

		SERIALIZABLE(CurveDeleteKeysAction);

	protected:
		CurveEditor::CurveKeysInfosVec mInfos;
		CurveEditor*                   mEditor;
	};

	class CurveKeysChangeAction : public IAction
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
		CurveKeysChangeAction();
		CurveKeysChangeAction(const KeysInfosVec& infos, CurveEditor* editor);

		String GetName();
		void Redo();
		void Undo();

		SERIALIZABLE(CurveKeysChangeAction);

	protected:
		KeysInfosVec   mInfos;
		CurveEditor*   mEditor;
	};
}

CLASS_BASES_META(Editor::CurveAddKeysAction)
{
	BASE_CLASS(Editor::IAction);
}
END_META;
CLASS_FIELDS_META(Editor::CurveAddKeysAction)
{
	PROTECTED_FIELD(mInfos);
	PROTECTED_FIELD(mEditor);
}
END_META;
CLASS_METHODS_META(Editor::CurveAddKeysAction)
{

	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(void, Redo);
	PUBLIC_FUNCTION(void, Undo);
}
END_META;

CLASS_BASES_META(Editor::CurveDeleteKeysAction)
{
	BASE_CLASS(Editor::IAction);
}
END_META;
CLASS_FIELDS_META(Editor::CurveDeleteKeysAction)
{
	PROTECTED_FIELD(mInfos);
	PROTECTED_FIELD(mEditor);
}
END_META;
CLASS_METHODS_META(Editor::CurveDeleteKeysAction)
{

	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(void, Redo);
	PUBLIC_FUNCTION(void, Undo);
}
END_META;

CLASS_BASES_META(Editor::CurveKeysChangeAction)
{
	BASE_CLASS(Editor::IAction);
}
END_META;
CLASS_FIELDS_META(Editor::CurveKeysChangeAction)
{
	PROTECTED_FIELD(mInfos);
	PROTECTED_FIELD(mEditor);
}
END_META;
CLASS_METHODS_META(Editor::CurveKeysChangeAction)
{

	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(void, Redo);
	PUBLIC_FUNCTION(void, Undo);
}
END_META;
