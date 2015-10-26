#include "Button.h"

#include "Render/Sprite.h"
#include "Render/Text.h"

namespace o2
{
	IOBJECT_CPP(Button);

	Button::Button():
		Widget()
	{
		InitializeProperties();
	}

	Button::Button(const Button& other):
		Widget(other)
	{
		InitializeProperties();
	}

	Button& Button::operator=(const Button& other)
	{
		Widget::operator=(other);
		return *this;
	}

	void Button::SetCaption(const WString& text)
	{
		FindCaptionLayerText()->SetText(text);
	}

	WString Button::GetCaption() const
	{
		auto captionText = GetLayerDrawable<Text>("caption");
		if (captionText)
			return captionText->GetText();

		return WString();
	}

	void Button::SetCCaption(const String& text)
	{
		FindCaptionLayerText()->SetCText(text);
	}

	String Button::GetCCaption() const
	{
		auto captionText = GetLayerDrawable<Text>("caption");
		if (captionText)
			return captionText->GetCText();

		return String();
	}

	void Button::SetIcon(Ptr<ImageAsset> asset)
	{
		FindIconLayerSprite()->LoadFromImage(asset);
	}

	void Button::SetIcon(const String& imagePath)
	{
		FindIconLayerSprite()->LoadFromImage(imagePath);
	}

	void Button::SetIcon(AssetId imageAssetId)
	{
		FindIconLayerSprite()->LoadFromImage(imageAssetId);
	}

	bool Button::IsUnderPoint(const Vec2F& point)
	{
		return layout.GetAbsoluteRect().IsInside(point);
	}

	float Button::Depth()
	{
		if (mDrawingLayers.Count() > 0)
			return mDrawingLayers.Last()->drawable->GetDrawingDepth();

		return 0.0f;
	}

	void Button::OnCursorPressed(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = true;
	}

	void Button::OnCursorReleased(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;

		if (IsUnderPoint(cursor.mPosition))
			onClick();
	}

	void Button::OnCursorEnter(const Input::Cursor& cursor)
	{
		auto selectState = state["select"];
		if (selectState)
			*selectState = true;
	}

	void Button::OnCursorExit(const Input::Cursor& cursor)
	{
		auto selectState = state["select"];
		if (selectState)
			*selectState = false;
	}

	Ptr<Text> Button::FindCaptionLayerText()
	{
		auto captionText = GetLayerDrawable<Text>("caption");
		if (!captionText)
		{
			auto layer = AddTextLayer("caption", "", "arial.ttf");
			return layer->drawable.Cast<Text>();
		}

		return captionText;
	}

	Ptr<Sprite> Button::FindIconLayerSprite()
	{
		auto iconSprite = GetLayerDrawable<Sprite>("icon");
		if (!iconSprite)
		{
			auto layer = AddSpriteLayer("icon", 0);
			return layer->drawable.Cast<Sprite>();
		}

		return iconSprite;
	}

	void Button::InitializeProperties()
	{
		INITIALIZE_PROPERTY(Button, caption, SetCaption, GetCaption);
		INITIALIZE_PROPERTY(Button, captionc, SetCCaption, GetCCaption);
		INITIALIZE_SETTER(Button, icon, SetIcon);
		INITIALIZE_SETTER(Button, iconPath, SetIcon);
		INITIALIZE_SETTER(Button, iconImageId, SetIcon);
	}
}