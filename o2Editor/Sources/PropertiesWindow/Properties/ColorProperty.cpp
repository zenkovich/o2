#include "ColorProperty.h"

#include "Application/Application.h"
#include "Core/Dialogs/ColorPickerDlg.h"
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
	ColorProperty::ColorProperty(UIWidget* widget /*= nullptr*/)
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

		Color4 color1(1.0f, 1.0f, 1.0f, 1.0f), color2(0.7f, 0.7f, 0.7f, 1.0f);
		Bitmap backLayerBitmap(Bitmap::Format::R8G8B8A8, Vec2I(20, 20));
		backLayerBitmap.Fill(color1);
		backLayerBitmap.FillRect(0, 0, 10, 10, color2);
		backLayerBitmap.FillRect(10, 10, 20, 20, color2);

		UIImage* backImage = mnew UIImage();
		backImage->image = mnew Sprite(&backLayerBitmap);
		backImage->GetImage()->mode = SpriteMode::Tiled;
		*backImage->layout = UIWidgetLayout::BothStretch(1, 1, 1, 1);
		mEditBox->AddChild(backImage);

		Bitmap colorLayerBitmap(Bitmap::Format::R8G8B8A8, Vec2I(20, 20));
		colorLayerBitmap.Fill(color1);
		mColorSprite = mnew UIImage();
		mColorSprite->image = mnew Sprite(&colorLayerBitmap);
		*mColorSprite->layout = UIWidgetLayout::BothStretch(1, 1, 1, 1);
		mEditBox->AddChild(mColorSprite);

		mEditBox->onDraw += [&]() { mClickArea.OnDrawn(); };
		mClickArea.isUnderPoint = [&](const Vec2F& point) { return mEditBox->IsUnderPoint(point); };
		mClickArea.onCursorReleased = [&](const Input::Cursor& cursor) { if (mEditBox->IsUnderPoint(cursor.position)) OnClicked(); };
	}

	ColorProperty::~ColorProperty()
	{
		delete mPropertyWidget;
	}

	void ColorProperty::SetValueAndPrototypePtr(const TargetsVec& targets, bool isProperty)
	{
		if (isProperty)
		{
			mAssignFunc = [](void* ptr, const Color4& value) { *((Property<Color4>*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return ((Property<Color4>*)(ptr))->Get(); };
		}
		else
		{
			mAssignFunc = [](void* ptr, const Color4& value) { *((Color4*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return *((Color4*)(ptr)); };
		}

		mValuesPointers = targets;

		Refresh();
	}

	void ColorProperty::Refresh()
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

	void ColorProperty::Revert()
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

	UIWidget* ColorProperty::GetWidget() const
	{
		return mPropertyWidget;
	}

	Color4 ColorProperty::GetCommonValue() const
	{
		return mCommonValue;
	}

	bool ColorProperty::IsValuesDifferent() const
	{
		return mValuesDifferent;
	}

	void ColorProperty::SetValue(const Color4& value)
	{
		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr.first, value);

		SetCommonValue(value);
	}

	void ColorProperty::SetUnknownValue(const Color4& defaultValue /*= Color4()*/)
	{
		mCommonValue = defaultValue;
		mValuesDifferent = true;
		mColorSprite->GetImage()->SetColor(mCommonValue);
		mColorSprite->transparency = mCommonValue.AF();

		OnChanged();
	}

	const Type* ColorProperty::GetFieldType() const
	{
		return &TypeOf(Color4);
	}

	void ColorProperty::SetCommonValue(const Color4& value)
	{
		mCommonValue = value;
		mValuesDifferent = false;
		mColorSprite->GetImage()->SetColor(mCommonValue);
		mColorSprite->transparency = mCommonValue.AF();

		OnChanged();
	}

	void ColorProperty::CheckRevertableState()
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

	void ColorProperty::OnClicked()
	{
		StoreValues(mBeforeChangeValues);

		ColorPickerDlg::Show(mCommonValue, 
							 THIS_FUNC(SetValue), 
							 THIS_FUNC(CheckValueChangeCompleted));
	}

	void ColorProperty::CheckValueChangeCompleted()
	{
		Vector<DataNode> valuesData;
		StoreValues(valuesData);

		if (mBeforeChangeValues != valuesData)
			onChangeCompleted(mValuesPath, mBeforeChangeValues, valuesData);
	}

	void ColorProperty::StoreValues(Vector<DataNode>& data) const
	{
		data.Clear();
		for (auto ptr : mValuesPointers)
		{
			data.Add(DataNode());
			data.Last() = mGetFunc(ptr.first);
		}
	}

}

CLASS_META(Editor::ColorProperty)
{
	BASE_CLASS(Editor::IPropertyField);

	PROTECTED_FIELD(mAssignFunc);
	PROTECTED_FIELD(mGetFunc);
	PROTECTED_FIELD(mValuesPointers);
	PROTECTED_FIELD(mCommonValue);
	PROTECTED_FIELD(mValuesDifferent);
	PROTECTED_FIELD(mPropertyWidget);
	PROTECTED_FIELD(mEditBox);
	PROTECTED_FIELD(mRevertBtn);
	PROTECTED_FIELD(mColorSprite);
	PROTECTED_FIELD(mClickArea);

	PUBLIC_FUNCTION(void, SetValueAndPrototypePtr, const TargetsVec&, bool);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(void, Revert);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(Color4, GetCommonValue);
	PUBLIC_FUNCTION(bool, IsValuesDifferent);
	PUBLIC_FUNCTION(void, SetValue, const Color4&);
	PUBLIC_FUNCTION(void, SetUnknownValue, const Color4&);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PROTECTED_FUNCTION(void, SetCommonValue, const Color4&);
	PROTECTED_FUNCTION(void, CheckRevertableState);
	PROTECTED_FUNCTION(void, OnClicked);
	PROTECTED_FUNCTION(void, CheckValueChangeCompleted);
	PROTECTED_FUNCTION(void, StoreValues, Vector<DataNode>&);
}
END_META;
