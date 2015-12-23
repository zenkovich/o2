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
		enum class HorOverflow { Cut, Dots, Expand, Wrap, None };
		enum class VerOverflow { Cut, None, Expand };

	public:
		Property<FontRef>     font;                // Font pointer property
		Property<WString>     text;                // Text property, wstring
		Property<VerAlign>    verAlign;            // vertical align property
		Property<HorAlign>    horAlign;            // Horizontal align property
		Property<HorOverflow> horOverflow;		   // Horizontal text overflow logic property
		Property<VerOverflow> verOverflow;		   // Vertical text overflow logic property
		Property<float>       symbolsDistanceCoef; // Characters distance coef, 1 is standard
		Property<float>       linesDistanceCoef;   // Lines distance coef, 1 is standard

		// Default constructor
		UILabel();

		// Copy-constructor
		UILabel(const UILabel& other);

		// Assign operator
		UILabel& operator=(const UILabel& other);

		// Draws widget
		void Draw();

		// Sets using font
		void SetFont(FontRef font);

		// Returns using font
		FontRef GetFont() const;

		// Sets text
		void SetText(const WString& text);

		// Returns text
		WString GetText() const;

		// Sets horizontal align
		void SetHorAlign(HorAlign align);

		// Returns horizontal align
		HorAlign GetHorAlign() const;

		// Sets vertical align
		void SetVerAlign(VerAlign align);

		// returns vertical align
		VerAlign GetVerAlign() const;

		// Sets horizontal overflow logic
		void SetHorOverflow(HorOverflow overflow);

		// Returns horizontal overflow logic
		HorOverflow GetHorOverflow();

		// Sets vertical overflow logic
		void SetVerOverflow(VerOverflow overflow);

		// Returns vertical overflow logic
		VerOverflow GetVerOverflow();

		// Sets characters distance coefficient
		void SetSymbolsDistanceCoef(float coef = 1);

		// Returns characters distance coef
		float GetSymbolsDistanceCoef() const;

		// Sets lines distance coefficient
		void SetLinesDistanceCoef(float coef = 1);

		// Returns lines distance coefficient
		float GetLinesDistanceCoef() const;

		SERIALIZABLE(UILabel);

	protected:
		Ptr<Text>   mTextLayer;   // Text layer drawable. Getting from layer "text"
		HorOverflow mHorOverflow; // Text horizontal overflow logic
		VerOverflow mVerOverflow; // Text vertical overflow logic

	protected:
		// Updates layout
		void UpdateLayout(bool forcible = false);

		// Calls when layer added and updates drawing sequence
		void OnLayerAdded(Ptr<UIWidgetLayer> layer);

		// Initializes properties
		void InitializeProperties();
	};
}
