#pragma once

#include "UI/Widget.h"
#include "Render/Text.h"

namespace o2
{
	// -----------------
	// Text label widget
	// -----------------
	class UILabel: public UIWidget
	{
	public:
		Property<FontRef>        font;                // Font pointer property
		Property<WString>        text;                // Text property, wstring
		Property<Text::VerAlign> verAlign;            // vertical align property
		Property<Text::HorAlign> horAlign;            // Horizontal align property
		Property<bool>           wordWrap;            // Words wrapping flag property
		Property<float>          symbolsDistanceCoef; // Characters distance coef, 1 is standard
		Property<float>          linesDistanceCoef;   // Lines distance coef, 1 is standard

		// Default constructor
		UILabel();

		// Copy-constructor
		UILabel(const UILabel& other);

		// Assign operator
		UILabel& operator=(const UILabel& other);

		// Sets using font
		void SetFont(FontRef font);

		// Returns using font
		FontRef GetFont() const;

		// Sets text
		void SetText(const WString& text);

		// Returns text
		WString GetText() const;

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

		SERIALIZABLE_IMPL(UILabel);

		IOBJECT(UILabel)
		{
			BASE_CLASS(UIWidget);
		}

	protected:
		Ptr<Text> mTextLayer; // Text layer drawable. Getting from layer "text"

	protected:
		// Calls when layer added and updates drawing sequence
		void OnLayerAdded(Ptr<UIWidgetLayer> layer);

		// Initializes properties
		void InitializeProperties();
	};
}
