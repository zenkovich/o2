#include "stdafx.h"
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
		backLayerBitmap.FillRect(0, 10, 10, 0, color2);
		backLayerBitmap.FillRect(10, 20, 20, 10, color2);

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

	void ColorProperty::SetValueAndPrototypeProxy(const TargetsVec& targets)
	{
		mValuesProxies = targets;

		Refresh();
	}

	void ColorProperty::Refresh()
	{
		if (mValuesProxies.IsEmpty())
			return;

		auto lastCommonValue = mCommonValue;
		auto lastDifferent = mValuesDifferent;

		auto newCommonValue = GetProxy<Color4>(mValuesProxies[0].first);
		auto newDifferent = false;

		for (int i = 1; i < mValuesProxies.Count(); i++)
		{
			if (newCommonValue != GetProxy<Color4>(mValuesProxies[i].first))
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
		for (auto ptr : mValuesProxies)
		{
			if (ptr.second)
			{
				SetProxy<Color4>(ptr.first, GetProxy<Color4>(ptr.second));
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
		for (auto ptr : mValuesProxies)
			SetProxy<Color4>(ptr.first, value);

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

		for (auto ptr : mValuesProxies)
		{
			if (ptr.second && !Math::Equals(GetProxy<Color4>(ptr.first), GetProxy<Color4>(ptr.second)))
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
		for (auto ptr : mValuesProxies)
		{
			data.Add(DataNode());
			data.Last() = GetProxy<Color4>(ptr.first);
		}
	}

}

DECLARE_CLASS(Editor::ColorProperty);
