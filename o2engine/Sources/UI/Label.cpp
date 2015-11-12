#include "Label.h"

namespace o2
{
	IOBJECT_CPP(UILabel);

	UILabel::UILabel()
	{
		InitializeProperties();
	}

	UILabel::UILabel(const UILabel& other):
		UIWidget(other)
	{
		mTextLayer = GetLayerDrawable<Text>("text");
		InitializeProperties();
	}

	UILabel& UILabel::operator=(const UILabel& other)
	{
		UIWidget::operator=(other);
		mTextLayer = GetLayerDrawable<Text>("text");
		return *this;
	}

	void UILabel::SetFont(FontRef font)
	{
		if (mTextLayer)
			mTextLayer->SetFont(font);
	}

	FontRef UILabel::GetFont() const
	{
		if (mTextLayer)
			return mTextLayer->GetFont();

		return FontRef();
	}

	void UILabel::SetText(const WString& text)
	{
		if (mTextLayer)
			mTextLayer->SetText(text);
	}

	WString UILabel::GetText() const
	{
		if (mTextLayer)
			return mTextLayer->GetText();

		return WString();
	}

	void UILabel::SetHorAlign(Text::HorAlign align)
	{
		if (mTextLayer)
			mTextLayer->SetHorAlign(align);
	}

	Text::HorAlign UILabel::GetHorAlign() const
	{
		if (mTextLayer)
			return mTextLayer->GetHorAlign();

		return Text::HorAlign::Left;
	}

	void UILabel::SetVerAlign(Text::VerAlign align)
	{
		if (mTextLayer)
			mTextLayer->SetVerAlign(align);
	}

	Text::VerAlign UILabel::GetVerAlign() const
	{
		if (mTextLayer)
			return mTextLayer->GetVerAlign();

		return Text::VerAlign::Top;
	}

	void UILabel::SetWordWrap(bool flag)
	{
		if (mTextLayer)
			mTextLayer->SetWordWrap(flag);
	}

	bool UILabel::GetWordWrap() const
	{
		if (mTextLayer)
			return mTextLayer->GetWordWrap();

		return false;
	}

	void UILabel::SetSymbolsDistanceCoef(float coef /*= 1*/)
	{
		if (mTextLayer)
			mTextLayer->SetSymbolsDistanceCoef(coef);
	}

	float UILabel::GetSymbolsDistanceCoef() const
	{
		if (mTextLayer)
			return mTextLayer->GetSymbolsDistanceCoef();

		return 1.0f;
	}

	void UILabel::SetLinesDistanceCoef(float coef /*= 1*/)
	{
		if (mTextLayer)
			mTextLayer->SetLinesDistanceCoef(coef);
	}

	float UILabel::GetLinesDistanceCoef() const
	{
		if (mTextLayer)
			return mTextLayer->GetLinesDistanceCoef();

		return 1.0f;
	}

	void UILabel::OnLayerAdded(Ptr<UIWidgetLayer> layer)
	{
		if (layer->name == "text" && layer->drawable && layer->drawable->GetTypeId() == Text::type->ID())
			mTextLayer = layer->drawable.Cast<Text>();
	}

	void UILabel::InitializeProperties()
	{
		INITIALIZE_PROPERTY(UILabel, font, SetFont, GetFont);
		INITIALIZE_PROPERTY(UILabel, text, SetText, GetText);
		INITIALIZE_PROPERTY(UILabel, verAlign, SetVerAlign, GetVerAlign);
		INITIALIZE_PROPERTY(UILabel, horAlign, SetHorAlign, GetHorAlign);
		INITIALIZE_PROPERTY(UILabel, wordWrap, SetWordWrap, GetWordWrap);
		INITIALIZE_PROPERTY(UILabel, symbolsDistanceCoef, SetSymbolsDistanceCoef, GetSymbolsDistanceCoef);
		INITIALIZE_PROPERTY(UILabel, linesDistanceCoef, SetLinesDistanceCoef, GetLinesDistanceCoef);
	}
}