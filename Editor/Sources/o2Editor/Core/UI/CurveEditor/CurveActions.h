#pragma once

#include "o2Editor/Core/Actions/IAction.h"
#include "o2Editor/Core/UI/CurveEditor/CurvesEditor.h"

namespace Editor
{
	class CurveAddKeysAction : public IAction
	{
	public:
		CurveAddKeysAction();
		CurveAddKeysAction(const Vector<CurvesEditor::CurveKeysInfo>& infos, CurvesEditor* editor);

		String GetName();
		void Redo();
		void Undo();

		SERIALIZABLE(CurveAddKeysAction);

	protected:
		Vector<CurvesEditor::CurveKeysInfo> mInfos;
		CurvesEditor*                       mEditor;
	};

	class CurveDeleteKeysAction : public IAction
	{
	public:
		CurveDeleteKeysAction();
		CurveDeleteKeysAction(const Vector<CurvesEditor::CurveKeysInfo>& infos, CurvesEditor* editor);

		String GetName();
		void Redo();
		void Undo();

		SERIALIZABLE(CurveDeleteKeysAction);

	protected:
		Vector<CurvesEditor::CurveKeysInfo> mInfos;
		CurvesEditor*                       mEditor;
	};

	class CurveKeysChangeAction : public IAction
	{
	public:
		struct KeysInfo
		{
			String curveId;

			Vector<Curve::Key> beforeKeys;
			Vector<Curve::Key> afterKeys;

			Vector<CurvesEditor::SelectedHandlesInfo> selectedHandles;

			bool operator==(const KeysInfo& other) const;
		};

	public:
		CurveKeysChangeAction();
		CurveKeysChangeAction(const Vector<KeysInfo>& infos, CurvesEditor* editor);

		String GetName();
		void Redo();
		void Undo();

		SERIALIZABLE(CurveKeysChangeAction);

	protected:
		Vector<KeysInfo> mInfos;
		CurvesEditor*    mEditor;
	};
}
// --- META ---

CLASS_BASES_META(Editor::CurveAddKeysAction)
{
	BASE_CLASS(Editor::IAction);
}
END_META;
CLASS_FIELDS_META(Editor::CurveAddKeysAction)
{
	FIELD().PROTECTED().NAME(mInfos);
	FIELD().PROTECTED().NAME(mEditor);
}
END_META;
CLASS_METHODS_META(Editor::CurveAddKeysAction)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const Vector<CurvesEditor::CurveKeysInfo>&, CurvesEditor*);
	FUNCTION().PUBLIC().SIGNATURE(String, GetName);
	FUNCTION().PUBLIC().SIGNATURE(void, Redo);
	FUNCTION().PUBLIC().SIGNATURE(void, Undo);
}
END_META;

CLASS_BASES_META(Editor::CurveDeleteKeysAction)
{
	BASE_CLASS(Editor::IAction);
}
END_META;
CLASS_FIELDS_META(Editor::CurveDeleteKeysAction)
{
	FIELD().PROTECTED().NAME(mInfos);
	FIELD().PROTECTED().NAME(mEditor);
}
END_META;
CLASS_METHODS_META(Editor::CurveDeleteKeysAction)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const Vector<CurvesEditor::CurveKeysInfo>&, CurvesEditor*);
	FUNCTION().PUBLIC().SIGNATURE(String, GetName);
	FUNCTION().PUBLIC().SIGNATURE(void, Redo);
	FUNCTION().PUBLIC().SIGNATURE(void, Undo);
}
END_META;

CLASS_BASES_META(Editor::CurveKeysChangeAction)
{
	BASE_CLASS(Editor::IAction);
}
END_META;
CLASS_FIELDS_META(Editor::CurveKeysChangeAction)
{
	FIELD().PROTECTED().NAME(mInfos);
	FIELD().PROTECTED().NAME(mEditor);
}
END_META;
CLASS_METHODS_META(Editor::CurveKeysChangeAction)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const Vector<KeysInfo>&, CurvesEditor*);
	FUNCTION().PUBLIC().SIGNATURE(String, GetName);
	FUNCTION().PUBLIC().SIGNATURE(void, Redo);
	FUNCTION().PUBLIC().SIGNATURE(void, Undo);
}
END_META;
// --- END META ---
