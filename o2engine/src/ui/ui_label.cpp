#include "ui_label.h"

#include "render_system/text.h"

OPEN_O2_NAMESPACE

REGIST_TYPE(UILabel);

UILabel::UILabel(Font* font, const Layout& layout, const String& id /*= ""*/):
UIWidget(layout, id), mText(NULL)
{
	mText = mnew Text(font);

	mResTransparencyChanged.Add(callback<UILabel>(this, &UILabel::TransparencyChanged));
}

UILabel::UILabel(const UILabel& label):
UIWidget(label)
{
	mText = mnew Text(*label.mText);

	mResTransparencyChanged.Add(callback<UILabel>(this, &UILabel::TransparencyChanged));
}

UILabel::~UILabel()
{
	SafeRelease(mText);
}

UIWidget* UILabel::Clone() const
{
	return mnew UILabel(*this);
}

void UILabel::SetFont(Font* font)
{
	mText->SetFont(font);
}

Font* UILabel::GetFont() const
{
	return mText->GetFont();
}

void UILabel::SetText(const WString& text)
{
	mText->GetText(text);
}

WString UILabel::GetText() const
{
	return mText->GetText();
}

void UILabel::SetCText(const String& text)
{
	mText->SetCText(text);
}

String UILabel::GetCText() const
{
	return mText->GetCText();
}

Vec2F UILabel::GetTextRealSize()
{
	return mText->GetRealSize();
}

void UILabel::SetCharactersHeight(const float& height)
{
	mText->SetCharactersHeight(height);
}

float UILabel::GetCharactersHeight() const
{
	return mText->GetCharactersHeight();
}

void UILabel::SetHorAlign(const Font::HorAlign& align)
{
	mText->SetHorAlign(align);
}

Font::HorAlign UILabel::GetHorAlign() const
{
	return mText->GetHorAlign();
}

void UILabel::SetVerAlign(const Font::VerAlign& align)
{
	mText->SetVerAlign(align);
}

Font::VerAlign UILabel::GetVerAlign() const
{
	return mText->GetVerAlign();
}

void UILabel::SetWordWrap(const bool& flag)
{
	mText->SetWordWrap(flag);
}

bool UILabel::GetWordWrap() const
{
	return mText->GetWordWrap();
}

void UILabel::SetCharactersDistCoef(const float& coef /*= 1*/)
{
	mText->SetCharactersDistCoef(coef);
}

float UILabel::GetCharactersDistCoef() const
{
	return mText->GetCharactersDistCoef();
}

void UILabel::SetLinesDistCoef(const float& coef /*= 1*/)
{
	mText->SetLinesDistCoef(coef);
}

float UILabel::GetLinesDistCoef() const
{
	return mText->GetLinesDistCoef();
}

void UILabel::LocalDraw()
{
	mText->Draw();
}

void UILabel::LayoutUpdated()
{
	mText->GetRect(mLayout.GetRect());
}

void UILabel::TransparencyChanged()
{
	mText->SetTransparency(mResTransparency);
}

CLOSE_O2_NAMESPACE