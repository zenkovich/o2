#pragma once

#include "UI/Widget.h"
#include "Render/Text.h"

namespace o2
{
	// -----------------
	// Text label widget
	// -----------------
	class Label: public Widget
	{
	public:
		Property<FontRef>        font;                // Font pointer property
		Property<WString>        text;                // Text property, wstring
		Property<String>         ctext;               // Text property, string
		Property<Text::VerAlign> verAlign;            // vertical align property
		Property<Text::HorAlign> horAlign;            // Horizontal align property
		Property<bool>           wordWrap;            // Words wrapping flag property
		Property<float>          symbolsDistanceCoef; // Characters distance coef, 1 is standard
		Property<float>          linesDistanceCoef;   // Lines distance coef, 1 is standard

		// Default constructor
		Label();

		// Copy-constructor
		Label(const Label& other);

		// Assign operator
		Label& operator=(const Label& other);

		// Sets using font
		void SetFont(FontRef font);

		// Returns using font
		FontRef GetFont() const;

		// Sets bitmap font asset 
		void SetFontAsset(Ptr<BitmapFontAsset> asset);

		// Sets vector font asset
		void SetFontAsset(Ptr<VectorFontAsset> asset);

		// Sets font asset id (loads asset by this id)
		void SetFontAsset(AssetId assetId);

		// Sets font asset (loads asset by file name)
		void SetFontAsset(const String& fileName);

		// Returns asset by font asset id
		Ptr<Asset> GetFontAsset() const;

		// Returns font asset id
		AssetId GetFontAssetId() const;

		// Sets text
		void SetText(const WString& text);

		// Returns text
		WString GetText() const;

		// Sets text
		void SetCText(const String& text);

		// Returns text as string
		String GetCText() const;

		// Sets horizontal align
		void SetHorAlign(Text::HorAlign align);

		// Returns horizontal align
		Text::HorAlign GetHorAlign() const;

		// Sets vertical align
		void SetVerAlign(Text::VerAlign align);

		// returns vertical align
		Text::VerAlign GetVerAlign() const;

		// Sets word wrapping
		void SetWordWrap(bool flag);

		// Returns word wrapping
		bool GetWordWrap() const;

		// Sets characters distance coefficient
		void SetSymbolsDistanceCoef(float coef = 1);

		// Returns characters distance coef
		float GetSymbolsDistanceCoef() const;

		// Sets lines distance coefficient
		void SetLinesDistanceCoef(float coef = 1);

		// Returns lines distance coefficient
		float GetLinesDistanceCoef() const;

		SERIALIZABLE_IMPL(Label);

		IOBJECT(Label)
		{
			BASE_CLASS(Widget);
		}

	protected:
		Ptr<Text> mTextLayer; // Text layer drawable. Getting from layer "text"

	protected:
		// Need to search mtextLayer in "text" layer
		void SearchTextLayer(); 

		// Calls when added a new layer
		void OnLayerAdded(Ptr<WidgetLayer> layer);

		// Initializes properties
		void InitializeProperties();
	};
}
