#include "ui_sprite.h"

OPEN_O2_NAMESPACE

UISprite::UISprite(const Layout& layout, const String& id /*= ""*/):
UIWidget(layout, id)
{
	LayoutUpdated();

	mResTransparencyChanged.Add(callback<UISprite>(this, &UISprite::TransparencyChanged));
}

UISprite::UISprite(const UISprite& spriteWidget):
UIWidget(spriteWidget)
{
	mSprite = spriteWidget.mSprite;
	LayoutUpdated();

	transparency.onChangeEvent.add(callback<UISprite>(this, &UISprite::TransparencyChanged));
}

UISprite::~UISprite()
{
}

UIWidget* UISprite::Clone() const
{
	return mnew UISprite(*this);
}

void UISprite::LocalDraw()
{

	mSprite.Draw();
}

void UISprite::LayoutUpdated()
{
	mSprite.SetPosition(mGlobalPosition);
	mSprite.SetSize(mSize);
}

void UISprite::TransparencyChanged()
{
	mSprite.SetTransparency(mResTransparency);
}

CLOSE_O2_NAMESPACE