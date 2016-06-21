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
		Property<Vec2F>       expandBorder;        // Overflow expanding border size property
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

		// Sets overflow expanding border
		void SetExpandBorder(const Vec2F& border);

		// Returns expanding overflow border
		Vec2F GetExpandBorder() const;

		SERIALIZABLE(UILabel);

	protected:
		Text*       mTextLayer = nullptr;             // Text layer drawable. Getting from layer "text"
		HorOverflow mHorOverflow = HorOverflow::None; // Text horizontal overflow logic @SERIALIZABLE
		VerOverflow mVerOverflow = VerOverflow::None; // Text vertical overflow logic @SERIALIZEBLE
		Vec2F       mExpandBorder;                    // Expand overflow border size @SERIALIZABLE

	protected:
		// Updates layout
		void UpdateLayout(bool forcible = false, bool withChildren = true);

		// Calls when layer added and updates drawing sequence
		void OnLayerAdded(UIWidgetLayer* layer);

		// Initializes properties
		void InitializeProperties();
	};
}
