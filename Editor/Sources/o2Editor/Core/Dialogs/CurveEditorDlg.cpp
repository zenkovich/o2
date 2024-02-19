#include "o2Editor/stdafx.h"
#include "CurveEditorDlg.h"

#include "o2/Scene/UI/UIManager.h"
#include "o2/Scene/UI/WidgetLayer.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Scene/UI/Widgets/Window.h"
#include "o2Editor/Core/UI/CurveEditor/CurvesEditor.h"
#include "o2Editor/Core/UIRoot.h"

DECLARE_SINGLETON(Editor::CurveEditorDlg);

namespace Editor
{
	CurveEditorDlg::CurveEditorDlg()
	{
		mWindow = DynamicCast<o2::Window>(EditorUIRoot.AddWidget(o2UI.CreateWindow("Curve editor")));

		InitializeControls();

		mWindow->Hide(true);
		mWindow->layout->size = Vec2F(600, 500);

		mWindow->GetBackCursorListener().onCursorReleased = [&](const Input::Cursor& c) { OnCursorPressedOutside(); };
		mWindow->onHide = MakeFunction(this, &CurveEditorDlg::OnHide);
	}

	CurveEditorDlg::~CurveEditorDlg()
	{}

	void CurveEditorDlg::OnHide()
	{
		mOnChangedCallback();
		mOnChangeCompletedCallback();
	}

	void CurveEditorDlg::InitializeControls()
	{
		mEditorWidget = mmake<CurvesEditor>();
		*mEditorWidget->layout = WidgetLayout::BothStretch(0, 5, 5, 0);

		auto horScroll = o2UI.CreateHorScrollBar();
		*horScroll->layout = WidgetLayout::HorStretch(VerAlign::Bottom, 0, 0, 10, -10);
		mEditorWidget->SetHorScrollbar(horScroll);

		auto verScroll = o2UI.CreateVerScrollBar();
		*verScroll->layout = WidgetLayout::VerStretch(HorAlign::Right, 0, 0, 10, -10);
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

	void CurveEditorDlg::Show(const Function<void()>& onChanged, const Function<void()>& onCompleted /*= Function<void()>()*/)
	{
		mInstance->mWindow->ShowModal();
		mInstance->mOnChangedCallback = onChanged;
		mInstance->mOnChangeCompletedCallback = onCompleted;
		mInstance->mEditorWidget->RemoveAllCurves();
	}

	void CurveEditorDlg::AddEditingCurve(const String& id, const Ref<Curve>& curve, const Color4& color /*= Color4::Green()*/)
	{
		mInstance->mEditorWidget->AddCurve(id, curve, color);
	}

	void CurveEditorDlg::RemoveEditingCurve(const Ref<Curve>& curve)
	{
		mInstance->mEditorWidget->RemoveCurve(curve);
	}

	void CurveEditorDlg::RemoveEditingCurve(const String& id)
	{
		mInstance->mEditorWidget->RemoveCurve(id);
	}

	void CurveEditorDlg::RemoveAllEditingCurves()
	{
		mInstance->mEditorWidget->RemoveAllCurves();
	}

	void CurveEditorDlg::AddCurvesRange(const Ref<Curve>& curveA, const Ref<Curve>& curveB, const Color4& color /*= Color4::Green()*/)
	{
		mInstance->mEditorWidget->AddCurvesRange(curveA, curveB, color);
	}

	void CurveEditorDlg::RemoveCurvesRange(const Ref<Curve>& curveA, const Ref<Curve>& curveB)
	{
		mInstance->mEditorWidget->RemoveCurvesRange(curveA, curveB);
	}

	void CurveEditorDlg::OnCursorPressedOutside()
	{
		mOnChangedCallback();
		mWindow->Hide();
	}

}
