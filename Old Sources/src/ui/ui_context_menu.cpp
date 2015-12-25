#include "ui_context_menu.h"

#include "ui_button.h"
#include "ui_label.h"
#include "ui_sprite.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(UIContextMenu);

UIContextMenu::UIContextMenu(const Layout& layout, const String& id /*= ""*/):
UIDrawablesListWidget(layout, id)
{

}

UIContextMenu::UIContextMenu(const UIContextMenu& contextMenu):
UIDrawablesListWidget(contextMenu)
{
}

UIContextMenu::~UIContextMenu()
{
	SafeRelease(mButtonSample);
	SafeRelease(mDelimerSample);
}

UIWidget* UIContextMenu::Clone() const
{
	return mnew UIContextMenu(*this);
}

UIButton* UIContextMenu::AddButtonElement(const WString& caption, const String& key /*= ""*/,
										  const String& iconTextureName /*= ""*/, int position /*= -1*/)
{
	UIButton* newButton = mnew UIButton(*mButtonSample);
	newButton->SetCaption(caption);

	UILabel* keyLabel = newButton->GetWidgetByType<UILabel>("hintKeyLabel");
	if (keyLabel)
		keyLabel->SetCText(key);

	UISprite* iconSprite = newButton->GetWidgetByType<UISprite>("iconSprite");
	if (iconSprite)
	{
		if (iconTextureName.empty())
			iconSprite->SetVisible(false);
		else
			iconSprite->mSprite.SetTexture(TextureRef::CreateFromFile(iconTextureName));
	}

	AddChild(newButton, position);

	return newButton;
}

UIWidget* UIContextMenu::AddDelimer(int position /*= -1*/)
{
	UIWidget* newDelimer = mDelimerSample->Clone();

	AddChild(newDelimer, position);

	return newDelimer;
}

UIWidget* UIContextMenu::AddElement(UIWidget* widget, int position /*= -1*/)
{
	AddChild(widget, position);
	return widget;
}

void UIContextMenu::SetupElementsSamples(UIWidget* delimer, UIButton* button)
{
	mDelimerSample = delimer->Clone();
	mButtonSample = mnew UIButton(*button);
}

void UIContextMenu::LayoutUpdated()
{

}

bool UIContextMenu::LocalProcessInputMessage(const InputMessage& msg)
{
	return false;
}

CLOSE_O2_NAMESPACE