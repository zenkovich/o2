#include "stdafx.h"
#include "CurveEditorDlg.h"

#include "Core/UI/CurveEdit.h"
#include "Core/UIRoot.h"
#include "Scene/UI/UIManager.h"
#include "Scene/UI/WidgetLayer.h"
#include "Scene/UI/WidgetLayout.h"
#include "Scene/UI/Widgets/Window.h"

DECLARE_SINGLETON(Editor::CurveEditorDlg);

namespace Editor
{
	CurveEditorDlg::CurveEditorDlg()
	{
		mWindow = dynamic_cast<UIWindow*>(EditorUIRoot.AddWidget(o2UI.CreateWindow("Curve editor")));

		InitializeControls();

		mWindow->Hide(true);
		mWindow->layout->size = Vec2F(600, 500);

		mWindow->GetBackCursorListener().onCursorReleased = [&](const Input::Cursor& c) { OnCursorPressedOutside(); };
		mWindow->onHide = Func(this, &CurveEditorDlg::OnHide);
	}

	CurveEditorDlg::~CurveEditorDlg()
	{
		if (mWindow)
			delete mWindow;
	}

	void CurveEditorDlg::OnHide()
	{
		mOnChangedCallback();
		mOnChangeCompletedCallback();
	}

	void CurveEditorDlg::InitializeControls()
	{
		mEditorWidget = mnew UICurveEditor();
		*mEditorWidget->layout = UIWidgetLayout::BothStretch(0, 5, 5, 0);

		auto horScroll = o2UI.CreateHorScrollBar();
		*horScroll->layout = UIWidgetLayout::HorStretch(VerAlign::Bottom, 0, 0, 10, -10);
		mEditorWidget->SetHorScrollbar(horScroll);

		auto verScroll = o2UI.CreateVerScrollBar();
		*verScroll->layout = UIWidgetLayout::VerStretch(HorAlign::Right, 0, 0, 10, -10);
		mEditorWidget->SetVerScrollbar(verScroll);

		mEditorWidget->SetMainHandleImages(ImageAssetRef("ui/CurveHandle.png"),
										   ImageAssetRef("ui/CurveHandleHover.png"),
										   ImageAssetRef("ui/CurveHandlePressed.png"),
										   ImageAssetRef("ui/CurveHandleSelected.png"));

		mEditorWidget->SetSupportHandleImages(ImageAssetRef("ui/CurveSupportHandle.png"),
											  ImageAssetRef("ui/CurveSupportHandleHover.png"),
											  ImageAssetRef("ui/CurveSupportHandlePressed.png"),
											  ImageAssetRef("ui/CurveSupportHandleSelected.png"));

		mEditorWidget->SetSelectionSpriteImage(ImageAssetRef("ui/UI_Window_place.png"));

		mWindow->AddChild(mEditorWidget);
	}

	void CurveEditorDlg::Show(Function<void()> onChanged, Function<void()> onCompleted /*= Function<void()>()*/)
	{
		mInstance->mWindow->ShowModal();
		mInstance->mOnChangedCallback = onChanged;
		mInstance->mOnChangeCompletedCallback = onCompleted;
		mInstance->mEditorWidget->RemoveAllEditingCurves();
	}

	void CurveEditorDlg::AddEditingCurve(const String& id, Curve* curve, const Color4& color /*= Color4::Green()*/)
	{
		mInstance->mEditorWidget->AddEditingCurve(id, curve, color);
	}

	void CurveEditorDlg::RemoveEditingCurve(Curve* curve)
	{
		mInstance->mEditorWidget->RemoveEditingCurve(curve);
	}

	void CurveEditorDlg::RemoveEditingCurve(const String& id)
	{
		mInstance->mEditorWidget->RemoveEditingCurve(id);
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