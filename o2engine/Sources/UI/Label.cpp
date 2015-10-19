#include "Label.h"

namespace o2
{

	Label::Label()
	{
		InitializeProperties();
	}

	Label::Label(const Label& other):
		Widget(other)
	{
		SearchTextLayer();
		InitializeProperties();
	}

	Label& Label::operator=(const Label& other)
	{
		Widget::operator=(other);
		SearchTextLayer();

		return *this;
	}

	void Label::SetFont(FontRef font)
	{
		if (mTextLayer)
			mTextLayer->SetFont(font);
	}

	FontRef Label::GetFont() const
	{
		if (mTextLayer)
			return mTextLayer->GetFont();

		return FontRef();
	}

	void Label::SetFontAsset(Ptr<BitmapFontAsset> asset)
	{
		if (mTextLayer)
			mTextLayer->SetFontAsset(asset);
	}

	void Label::SetFontAsset(Ptr<VectorFontAsset> asset)
	{
		if (mTextLayer)
			mTextLayer->SetFontAsset(asset);
	}

	void Label::SetFontAsset(AssetId assetId)
	{
		if (mTextLayer)
			mTextLayer->SetFontAsset(assetId);
	}

	void Label::SetFontAsset(const String& fileName)
	{
		if (mTextLayer)
			mTextLayer->SetFontAsset(fileName);
	}

	Ptr<Asset> Label::GetFontAsset() const
	{
		if (mTextLayer)
			return mTextLayer->GetFontAsset();

		return nullptr;
	}

	AssetId Label::GetFontAssetId() const
	{
		if (mTextLayer)
			return mTextLayer->GetFontAssetId();

		return 0;
	}

	void Label::SetText(const WString& text)
	{
		if (mTextLayer)
			mTextLayer->SetText(text);
	}

	WString Label::GetText() const
	{
		if (mTextLayer)
			return mTextLayer->GetText();

		return WString();
	}

	void Label::SetCText(const String& text)
	{
		if (mTextLayer)
			mTextLayer->SetCText(text);
	}

	String Label::GetCText() const
	{
		if (mTextLayer)
			return mTextLayer->GetCText();

		return WString();
	}

	void Label::SetHorAlign(Text::HorAlign align)
	{
		if (mTextLayer)
			mTextLayer->SetHorAlign(align);
	}

	Text::HorAlign Label::GetHorAlign() const
	{
		if (mTextLayer)
			return mTextLayer->GetHorAlign();

		return Text::HorAlign::Left;
	}

	void Label::SetVerAlign(Text::VerAlign align)
	{
		if (mTextLayer)
			mTextLayer->SetVerAlign(align);
	}

	Text::VerAlign Label::GetVerAlign() const
	{
		if (mTextLayer)
			return mTextLayer->GetVerAlign();

		return Text::VerAlign::Top;
	}

	void Label::SetWordWrap(bool flag)
	{
		if (mTextLayer)
			mTextLayer->SetWordWrap(flag);
	}

	bool Label::GetWordWrap() const
	{
		if (mTextLayer)
			return mTextLayer->GetWordWrap();

		return false;
	}

	void Label::SetSymbolsDistanceCoef(float coef /*= 1*/)
	{
		if (mTextLayer)
			mTextLayer->SetSymbolsDistanceCoef(coef);
	}

	float Label::GetSymbolsDistanceCoef() const
	{
		if (mTextLayer)
			return mTextLayer->GetSymbolsDistanceCoef();

		return 1.0f;
	}

	void Label::SetLinesDistanceCoef(float coef /*= 1*/)
	{
		if (mTextLayer)
			mTextLayer->SetLinesDistanceCoef(coef);
	}

	float Label::GetLinesDistanceCoef() const
	{
		if (mTextLayer)
			return mTextLayer->GetLinesDistanceCoef();

		return 1.0f;
	}

	void Label::SearchTextLayer()
	{
		if (mTextLayer)
			return;

		auto textLayer = GetLayer("text");
		if (textLayer)
			mTextLayer = textLayer->drawable.Cast<Text>();
	}

	void Label::OnLayerAdded(Ptr<WidgetLayer> layer)
	{
		if (layer->id == "text" && layer->drawable->GetTypeId() == Text::type.ID())
			mTextLayer = layer->drawable.Cast<Text>();
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