#include "o2Editor/stdafx.h"
#include "CurveProperty.h"

#include "o2/Scene/UI/Widget.h"
#include "o2/Scene/UI/WidgetLayout.h"
#include "o2Editor/Core/Dialogs/CurveEditorDlg.h"
#include "o2Editor/Core/UI/CurvePreview.h"

namespace Editor
{
	CurveProperty::CurveProperty()
	{}

	CurveProperty::CurveProperty(const CurveProperty& other) :
		TPropertyField<Curve>(other)
	{
		InitializeControls();
	}

	CurveProperty& CurveProperty::operator=(const CurveProperty& other)
	{
		TPropertyField<Curve>::operator=(other);
		InitializeControls();
		return *this;
	}

	void CurveProperty::InitializeControls()
	{
		mEditBox = GetChildWidget("container/layout/box");
		if (mEditBox)
		{
			mPreviewImage = mmake<CurvePreview>();
			*mPreviewImage->layout = WidgetLayout::BothStretch(1, 1, 1, 1);
			//mPreviewImage->SetCurve(&mCommonValue);

			mEditBox->layout->minHeight = 10;
			mEditBox->onDraw += [&]() { mClickArea.OnDrawn(); };
			mEditBox->AddChild(mPreviewImage);

			mClickArea.isUnderPoint = [&](const Vec2F& point) { return mEditBox->IsUnderPoint(point); };
			mClickArea.onCursorReleased = [&](const Input::Cursor& cursor) { if (mEditBox->IsUnderPoint(cursor.position)) OnClicked(); };
		}
	}

	void CurveProperty::OnValueChanged()
	{
		for (auto& ptr : mValuesProxies)
			SetProxy(ptr.first, mCommonValue);
	}

	void CurveProperty::OnClicked()
	{
		StoreValues(mBeforeChangeValues);

		CurveEditorDlg::Show(THIS_FUNC(OnValueChanged),
							 MakeFunction<IPropertyField, void>(this, &CurveProperty::CheckValueChangeCompleted));

		CurveEditorDlg::RemoveAllEditingCurves();
		//CurveEditorDlg::AddEditingCurve("property", &mCommonValue, Color4(44, 62, 80));
	}
}
DECLARE_TEMPLATE_CLASS(Editor::TPropertyField<o2::Curve>);
// --- META ---

DECLARE_CLASS(Editor::CurveProperty, Editor__CurveProperty);
// --- END META ---
