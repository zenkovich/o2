#include "ui_rect.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(UIRect);

UIRect::UIRect(const Layout& layout, const String& id /*= ""*/):
UIWidget(layout, id)
{
	transparency.onChangeEvent.add(callback<UIRect>(this, &UIRect::TransparencyChanged));
}

UIRect::UIRect(const UIRect& rectWidget):
UIWidget(rectWidget)
{
	mStretchRect = rectWidget.mStretchRect;
	mResTransparencyChanged.Add(callback<UIRect>(this, &UIRect::TransparencyChanged));
}

UIRect::~UIRect()
{
}

UIWidget* UIRect::Clone() const
{
	return mnew UIRect(*this);
}

void UIRect::LocalDraw()
{
	mStretchRect.Draw();
}

void UIRect::LayoutUpdated()
{
	mStretchRect.GetRect(mBounds);
}

void UIRect::TransparencyChanged()
{
	mStretchRect.SetTransparency(mResTransparency);
}

CLOSE_O2_NAMESPACE