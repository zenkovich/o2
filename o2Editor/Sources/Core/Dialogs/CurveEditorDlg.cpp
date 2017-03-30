#include "CurveEditorDlg.h"

#include "Core/UI/CurveEdit.h"
#include "UI/UIManager.h"
#include "UI/Window.h"

DECLARE_SINGLETON(Editor::CurveEditorDlg);

namespace Editor
{
	CurveEditorDlg::CurveEditorDlg()
	{
		mWindow = o2UI.AddWindow("Curve editor");

		InitializeControls();

		mWindow->Hide(true);
		mWindow->layout.size = Vec2F(400, 300);

		mWindow->GetBackCursorListener().onCursorReleased = [&](const Input::Cursor& c) { OnCursorPressedOutside(); };
	}

	CurveEditorDlg::~CurveEditorDlg()
	{
		delete mWindow;
	}

	void CurveEditorDlg::InitializeControls()
	{
		mEditorWidget = mnew UICurveEditor();
		mEditorWidget->layout = UIWidgetLayout::BothStretch(5, 5, 5, 5);
		mWindow->AddChild(mEditorWidget);
	}

	void CurveEditorDlg::Show(Function<void()> onChanged)
	{
		mInstance->mWindow->ShowModal();
		mInstance->mOnChangedCallback = onChanged;
		mInstance->mEditorWidget->RemoveAllEditingCurves();
	}

	void CurveEditorDlg::AddEditingCurve(Curve* curve, const Color4& color /*= Color4::Green()*/)
	{
		mInstance->mEditorWidget->AddEditingCurve(curve, color);
	}

	void CurveEditorDlg::RemoveEditingCurve(Curve* curve)
	{
		mInstance->mEditorWidget->AddEditingCurve(curve);
	}

	void CurveEditorDlg::RemoveAllEditingCurves()
	{
		mInstance->mEditorWidget->RemoveAllEditingCurves();
	}

	void CurveEditorDlg::AddCurvesRange(Curve* curveA, Curve* curveB, const Color4& color /*= Color4::Green()*/)
	{
		mInstance->mEditorWidget->AddCurvesRange(curveA, curveB, color);
	}

	void CurveEditorDlg::RemoveCurvesRange(Curve* curveA, Curve* curveB)
	{
		mInstance->mEditorWidget->RemoveCurvesRange(curveA, curveB);
	}

	void CurveEditorDlg::OnCursorPressedOutside()
	{
		mOnChangedCallback();
		mWindow->Hide();
	}

}