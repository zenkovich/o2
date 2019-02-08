#pragma once

#include "Scene/UI/Widget.h"
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
		PROPERTIES(UILabel);
		PROPERTY(WString, text, SetText, GetText);   // Text property, wstring

		PROPERTY(FontRef, font, SetFont, GetFont);   // Font pointer property
		PROPERTY(int, height, SetHeight, GetHeight); // Text height property
		PROPERTY(Color4, color, SetColor, GetColor); // Text color property

		PROPERTY(VerAlign, verAlign, SetVerAlign, GetVerAlign); // vertical align property
		PROPERTY(HorAlign, horAlign, SetHorAlign, GetHorAlign); // Horizontal align property

		PROPERTY(HorOverflow, horOverflow, SetHorOverflow, GetHorOverflow); // Horizontal text overflow logic property
		PROPERTY(VerOverflow, verOverflow, SetVerOverflow, GetVerOverflow); // Vertical text overflow logic property

		PROPERTY(Vec2F, expandBorder, SetExpandBorder, GetExpandBorder); // Overflow expanding border size property

		PROPERTY(float, symbolsDistanceCoef, SetSymbolsDistanceCoef, GetSymbolsDistanceCoef); // Characters distance coef, 1 is standard
		PROPERTY(float, linesDistanceCoef, SetLinesDistanceCoef, GetLinesDistanceCoef);       // Lines distance coef, 1 is standard

		// Default constructor
		UILabel();

		// Copy-constructor
		UILabel(const UILabel& other);

		// Assign operator
		UILabel& operator=(const UILabel& other);

		// Draws widget
		void Draw() override;

		// Sets using font
		void SetFont(FontRef font);

		// Returns using font
		FontRef GetFont() const;

		// Sets text
		void SetText(const WString& text);

		// Returns text
		WString GetText() const;

		// Sets text color
		void SetColor(const Color4& color);

		// Returns color of text
		Color4 GetColor() const;

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

		// Sets text height
		void SetHeight(int height);

		// Returns text height
		int GetHeight() const;

		// Updates layout
		void UpdateSelfTransform();

		SERIALIZABLE(UILabel);

	protected:
		Text*       mTextLayer = nullptr;             // Text layer drawable. Getting from layer "text"
		HorOverflow mHorOverflow = HorOverflow::None; // Text horizontal overflow logic @SERIALIZABLE
		VerOverflow mVerOverflow = VerOverflow::None; // Text vertical overflow logic @SERIALIZABLE
		Vec2F       mExpandBorder;                    // Expand overflow border size @SERIALIZABLE

	protected:
		// Copies data of actor from other to this
		void CopyData(const Actor& otherActor) override;

		// It is called when layer added and updates drawing sequence
		void OnLayerAdded(UIWidgetLayer* layer) override;
	};
}

CLASS_BASES_META(o2::UILabel)
{
	BASE_CLASS(o2::UIWidget);
}
END_META;
CLASS_FIELDS_META(o2::UILabel)
{
	PUBLIC_FIELD(text);
	PUBLIC_FIELD(font);
	PUBLIC_FIELD(height);
	PUBLIC_FIELD(color);
	PUBLIC_FIELD(verAlign);
	PUBLIC_FIELD(horAlign);
	PUBLIC_FIELD(horOverflow);
	PUBLIC_FIELD(verOverflow);
	PUBLIC_FIELD(expandBorder);
	PUBLIC_FIELD(symbolsDistanceCoef);
	PUBLIC_FIELD(linesDistanceCoef);
	PROTECTED_FIELD(mTextLayer);
	PROTECTED_FIELD(mHorOverflow).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mVerOverflow).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mExpandBorder).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(o2::UILabel)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, SetFont, FontRef);
	PUBLIC_FUNCTION(FontRef, GetFont);
	PUBLIC_FUNCTION(void, SetText, const WString&);
	PUBLIC_FUNCTION(WString, GetText);
	PUBLIC_FUNCTION(void, SetColor, const Color4&);
	PUBLIC_FUNCTION(Color4, GetColor);
	PUBLIC_FUNCTION(void, SetHorAlign, HorAlign);
	PUBLIC_FUNCTION(HorAlign, GetHorAlign);
	PUBLIC_FUNCTION(void, SetVerAlign, VerAlign);
	PUBLIC_FUNCTION(VerAlign, GetVerAlign);
	PUBLIC_FUNCTION(void, SetHorOverflow, HorOverflow);
	PUBLIC_FUNCTION(HorOverflow, GetHorOverflow);
	PUBLIC_FUNCTION(void, SetVerOverflow, VerOverflow);
	PUBLIC_FUNCTION(VerOverflow, GetVerOverflow);
	PUBLIC_FUNCTION(void, SetSymbolsDistanceCoef, float);
	PUBLIC_FUNCTION(float, GetSymbolsDistanceCoef);
	PUBLIC_FUNCTION(void, SetLinesDistanceCoef, float);
	PUBLIC_FUNCTION(float, GetLinesDistanceCoef);
	PUBLIC_FUNCTION(void, SetExpandBorder, const Vec2F&);
	PUBLIC_FUNCTION(Vec2F, GetExpandBorder);
	PUBLIC_FUNCTION(void, SetHeight, int);
	PUBLIC_FUNCTION(int, GetHeight);
	PUBLIC_FUNCTION(void, UpdateSelfTransform);
	PROTECTED_FUNCTION(void, CopyData, const Actor&);
	PROTECTED_FUNCTION(void, OnLayerAdded, UIWidgetLayer*);
}
END_META;
