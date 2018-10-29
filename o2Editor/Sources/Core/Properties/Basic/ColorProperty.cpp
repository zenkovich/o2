#include "stdafx.h"
#include "ColorProperty.h"

#include "Application/Application.h"
#include "Core/Dialogs/ColorPickerDlg.h"
#include "SceneWindow/SceneEditScreen.h"
#include "UI/Image.h"
#include "UI/WidgetLayout.h"

namespace Editor
{
	ColorProperty::ColorProperty()
	{}

	ColorProperty::ColorProperty(const ColorProperty& other) :
		TPropertyField<Color4>(other)
	{
		InitializeControls();
	}

	ColorProperty& ColorProperty::operator=(const ColorProperty& other)
	{
		TPropertyField<Color4>::operator=(other);
		return *this;
	}

	void ColorProperty::CopyData(const Actor& otherActor)
	{
		TPropertyField<Color4>::CopyData(otherActor);
		InitializeControls();
	}

	void ColorProperty::InitializeControls()
	{
		mEditBox = GetChildWidget("layout/box");
		if (mEditBox)
		{
			mEditBox->layout->minHeight = 10;

			Color4 color1(1.0f, 1.0f, 1.0f, 1.0f), color2(0.7f, 0.7f, 0.7f, 1.0f);
			Bitmap backLayerBitmap(PixelFormat::R8G8B8A8, Vec2I(20, 20));
			backLayerBitmap.Fill(color1);
			backLayerBitmap.FillRect(0, 10, 10, 0, color2);
			backLayerBitmap.FillRect(10, 20, 20, 10, color2);

			UIImage* backImage = mnew UIImage();
			backImage->image = mnew Sprite(&backLayerBitmap);
			backImage->GetImage()->mode = SpriteMode::Tiled;
			*backImage->layout = UIWidgetLayout::BothStretch(1, 1, 1, 1);
			mEditBox->AddChild(backImage);

			Bitmap colorLayerBitmap(PixelFormat::R8G8B8A8, Vec2I(20, 20));
			colorLayerBitmap.Fill(color1);
			mColorSprite = mnew UIImage();
			mColorSprite->image = mnew Sprite(&colorLayerBitmap);
			*mColorSprite->layout = UIWidgetLayout::BothStretch(1, 1, 1, 1);
			mEditBox->AddChild(mColorSprite);

			mEditBox->onDraw += [&]() { mClickArea.OnDrawn(); };
			mClickArea.isUnderPoint = [&](const Vec2F& point) { return mEditBox->IsUnderPoint(point); };
			mClickArea.onCursorReleased = [&](const Input::Cursor& cursor) { if (mEditBox->IsUnderPoint(cursor.position)) OnClicked(); };
		}
	}

	void ColorProperty::UpdateValueView()
	{
		mColorSprite->GetImage()->SetColor(mCommonValue);
		mColorSprite->transparency = mCommonValue.AF();
	}

	void ColorProperty::OnClicked()
	{
		StoreValues(mBeforeChangeValues);

		ColorPickerDlg::Show(mCommonValue,
							 Func<TPropertyField<Color4>, void, const Color4&>(this, &ColorProperty::SetValue),
							 Func<TPropertyField<Color4>, void>(this, &ColorProperty::CheckValueChangeCompleted));
	}
}
DECLARE_CLASS_MANUAL(Editor::TPropertyField<o2::Color4>);

DECLARE_CLASS(Editor::ColorProperty);
