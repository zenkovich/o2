#include "Label.h"

namespace o2
{
	IOBJECT_CPP(Label);

	Label::Label()
	{
		InitializeProperties();
	}

	Label::Label(const Label& other):
		Widget(other)
	{
		InitializeProperties();
	}

	Label& Label::operator=(const Label& other)
	{
		Widget::operator=(other);

		return *this;
	}

	void Label::SetFont(FontRef font)
	{
		FindTextLayerText()->SetFont(font);
	}

	FontRef Label::GetFont() const
	{
		auto text = GetLayerDrawable<Text>("caption");
		if (text)
			return text->GetFont();

		return FontRef();
	}

	void Label::SetFontAsset(Ptr<BitmapFontAsset> asset)
	{
		FindTextLayerText()->SetFontAsset(asset);
	}

	void Label::SetFontAsset(Ptr<VectorFontAsset> asset)
	{
		FindTextLayerText()->SetFontAsset(asset);
	}

	void Label::SetFontAsset(AssetId assetId)
	{
		FindTextLayerText()->SetFontAsset(assetId);
	}

	void Label::SetFontAsset(const String& fileName)
	{
		FindTextLayerText()->SetFontAsset(fileName);
	}

	Ptr<Asset> Label::GetFontAsset() const
	{
		if (mTextLayer)
			return mTextLayer->GetFontAsset();

		return nullptr;
	}

	AssetId Label::GetFontAssetId() const
	{
		auto text = GetLayerDrawable<Text>("caption");
		if (text)
			return text->GetFontAssetId();

		return 0;
	}

	void Label::SetText(const WString& text)
	{
		FindTextLayerText()->SetText(text);
	}

	WString Label::GetText() const
	{
		auto text = GetLayerDrawable<Text>("caption");
		if (text)
			return text->GetText();

		return WString();
	}

	void Label::SetCText(const String& text)
	{
		FindTextLayerText()->SetCText(text);
	}

	String Label::GetCText() const
	{
		auto text = GetLayerDrawable<Text>("caption");
		if (text)
			return text->GetCText();

		return WString();
	}

	void Label::SetHorAlign(Text::HorAlign align)
	{
		FindTextLayerText()->SetHorAlign(align);
	}

	Text::HorAlign Label::GetHorAlign() const
	{
		auto text = GetLayerDrawable<Text>("caption");
		if (text)
			return text->GetHorAlign();

		return Text::HorAlign::Left;
	}

	void Label::SetVerAlign(Text::VerAlign align)
	{
		FindTextLayerText()->SetVerAlign(align);
	}

	Text::VerAlign Label::GetVerAlign() const
	{
		auto text = GetLayerDrawable<Text>("caption");
		if (text)
			return text->GetVerAlign();

		return Text::VerAlign::Top;
	}

	void Label::SetWordWrap(bool flag)
	{
		FindTextLayerText()->SetWordWrap(flag);
	}

	bool Label::GetWordWrap() const
	{
		auto text = GetLayerDrawable<Text>("caption");
		if (text)
			return text->GetWordWrap();

		return false;
	}

	void Label::SetSymbolsDistanceCoef(float coef /*= 1*/)
	{
		FindTextLayerText()->SetSymbolsDistanceCoef(coef);
	}

	float Label::GetSymbolsDistanceCoef() const
	{
		auto text = GetLayerDrawable<Text>("caption");
		if (text)
			return text->GetSymbolsDistanceCoef();

		return 1.0f;
	}

	void Label::SetLinesDistanceCoef(float coef /*= 1*/)
	{
		FindTextLayerText()->SetLinesDistanceCoef(coef);
	}

	float Label::GetLinesDistanceCoef() const
	{
		auto text = GetLayerDrawable<Text>("caption");
		if (text)
			return text->GetLinesDistanceCoef();

		return 1.0f;
	}

	Ptr<Text> Label::FindTextLayerText()
	{
		auto captionText = GetLayerDrawable<Text>("text");
		if (!captionText)
		{
			auto layer = AddTextLayer("text", "", "arial.ttf");
			return layer->drawable.Cast<Text>();
		}

		return captionText;
	}

	void Label::InitializeProperties()
	{
		INITIALIZE_PROPERTY(Label, font, SetFont, GetFont);
		INITIALIZE_PROPERTY(Label, text, SetText, GetText);
		INITIALIZE_PROPERTY(Label, ctext, SetCText, GetCText);
		INITIALIZE_PROPERTY(Label, verAlign, SetVerAlign, GetVerAlign);
		INITIALIZE_PROPERTY(Label, horAlign, SetHorAlign, GetHorAlign);
		INITIALIZE_PROPERTY(Label, wordWrap, SetWordWrap, GetWordWrap);
		INITIALIZE_PROPERTY(Label, symbolsDistanceCoef, SetSymbolsDistanceCoef, GetSymbolsDistanceCoef);
		INITIALIZE_PROPERTY(Label, linesDistanceCoef, SetLinesDistanceCoef, GetLinesDistanceCoef);
	}
}