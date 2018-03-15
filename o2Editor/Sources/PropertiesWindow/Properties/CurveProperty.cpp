#include "stdafx.h"
#include "CurveProperty.h"

#include "Application/Application.h"
#include "Core/Dialogs/CurveEditorDlg.h"
#include "Core/UI/CurvePreview.h"
#include "Render/Render.h"
#include "SceneWindow/SceneEditScreen.h"
#include "UI/Button.h"
#include "UI/EditBox.h"
#include "UI/Image.h"
#include "UI/UIManager.h"
#include "UI/WidgetLayer.h"
#include "UI/WidgetLayout.h"
#include "UI/WidgetState.h"

namespace Editor
{
	CurveProperty::CurveProperty(UIWidget* widget /*= nullptr*/)
	{
		if (widget)
			mPropertyWidget = widget;
		else
			mPropertyWidget = o2UI.CreateWidget<UIWidget>("color property");

		mEditBox = mPropertyWidget->GetChildWidget("box");
		mEditBox->layout->minHeight = 10;

		mRevertBtn = mPropertyWidget->GetChildByType<UIButton>();
		if (mRevertBtn)
			mRevertBtn->onClick = THIS_FUNC(Revert);

		mPreviewImage = mnew UICurvePreview();
		*mPreviewImage->layout = UIWidgetLayout::BothStretch(1, 1, 1, 1);
		mPreviewImage->SetCurve(&mCommonValue);
		mEditBox->AddChild(mPreviewImage);

		mEditBox->onDraw += [&]() { mClickArea.OnDrawn(); };
		mClickArea.isUnderPoint = [&](const Vec2F& point) { return mEditBox->IsUnderPoint(point); };
		mClickArea.onCursorReleased = [&](const Input::Cursor& cursor) { if (mEditBox->IsUnderPoint(cursor.position)) OnClicked(); };
	}

	CurveProperty::~CurveProperty()
	{
		delete mPropertyWidget;
	}

	void CurveProperty::SetValueAndPrototypePtr(const TargetsVec& targets, bool isProperty)
	{
		if (isProperty)
		{
			mAssignFunc = [](void* ptr, const Curve& value) { *((PROPERTY(Curve>*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return ((PROPERTY(Curve>*)(ptr))->Get(); };
		}
		else
		{
			mAssignFunc = [](void* ptr, const Curve& value) { *((Curve*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return *((Curve*)(ptr)); };
		}

		mValuesPointers = targets;

		Refresh();
	}

	void CurveProperty::Refresh()
	{
		if (mValuesPointers.IsEmpty())
			return;

		auto lastCommonValue = mCommonValue;
		auto lastDifferent = mValuesDifferent;

		auto newCommonValue = mGetFunc(mValuesPointers[0].first);
		auto newDifferent = false;

		for (int i = 1; i < mValuesPointers.Count(); i++)
		{
			if (newCommonValue != mGetFunc(mValuesPointers[i].first))
			{
				newDifferent = true;
				break;
			}
		}

		if (newDifferent)
		{
			if (!lastDifferent)
				SetUnknownValue(newCommonValue);
		}
		else if (!Math::Equals(lastCommonValue, newCommonValue) || lastDifferent)
			SetCommonValue(newCommonValue);

		CheckRevertableState();
	}

	void CurveProperty::Revert()
	{
		for (auto ptr : mValuesPointers)
		{
			if (ptr.second)
			{
				mAssignFunc(ptr.first, mGetFunc(ptr.second));
			}
		}

		Refresh();
	}

	UIWidget* CurveProperty::GetWidget() const
	{
		return mPropertyWidget;
	}

	const Curve& CurveProperty::GetCommonValue() const
	{
		return mCommonValue;
	}

	bool CurveProperty::IsValuesDifferent() const
	{
		return mValuesDifferent;
	}

	void CurveProperty::SetValue(const Curve& value)
	{
		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr.first, value);

		SetCommonValue(value);
	}

	void CurveProperty::SetUnknownValue(const Curve& defaultValue /*= Curve()*/)
	{
		mCommonValue = defaultValue;
		mValuesDifferent = true;

		OnChanged();
	}

	const Type* CurveProperty::GetFieldType() const
	{
		return &TypeOf(Curve);
	}

	void CurveProperty::OnValueChanged()
	{
		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr.first, mCommonValue);
	}

	void CurveProperty::SetCommonValue(const Curve& value)
	{
		mCommonValue = value;
		mValuesDifferent = false;

		OnChanged();
	}

	void CurveProperty::CheckRevertableState()
	{
		bool revertable = false;

		for (auto ptr : mValuesPointers)
		{
			if (ptr.second && !Math::Equals(mGetFunc(ptr.first), mGetFunc(ptr.second)))
			{
				revertable = true;
				break;
			}
		}

		if (auto state = mPropertyWidget->state["revert"])
			*state = revertable;
	}

	void CurveProperty::OnClicked()
	{
		StoreValues(mBeforeChangeValues);

		CurveEditorDlg::Show(THIS_FUNC(OnValueChanged),
							 THIS_FUNC(CheckValueChangeCompleted));

		CurveEditorDlg::RemoveAllEditingCurves();
		CurveEditorDlg::AddEditingCurve("property", &mCommonValue);
	}

	void CurveProperty::CheckValueChangeCompleted()
	{
		CurveEditorDlg::RemoveAllEditingCurves();

		Vector<DataNode> valuesData;
		StoreValues(valuesData);

		if (mBeforeChangeValues != valuesData)
			onChangeCompleted(mValuesPath, mBeforeChangeValues, valuesData);
	}

	void CurveProperty::StoreValues(Vector<DataNode>& data) const
	{
		data.Clear();
		for (auto ptr : mValuesPointers)
		{
			data.Add(DataNode());
			data.Last() = mGetFunc(ptr.first);
		}
	}

}

DECLARE_CLASS(Editor::CurveProperty);
