#pragma once

#include "ui_widget.h"
#include "render_system/font.h"

OPEN_O2_NAMESPACE

class Text;

class UILabel: public UIWidget
{
	Text* mText; /** Basic text drawable. */

public:
	DEFINE_TYPE(UILabel);

	//properties
	PROPERTY(UILabel, Font*)          font;                /** Font pointer property. Uses set/getFont. */
	PROPERTY(UILabel, WString)        text;                /** Text property, wstring. Uses set/getText. */
	PROPERTY(UILabel, String)         ctext;               /** Text property, string. Uses set/getCText. */
	PROPERTY(UILabel, Font::VerAlign) verAlign;            /** vertical align property. Uses set/getVerAlign. */
	PROPERTY(UILabel, Font::HorAlign) horAlign;            /** Horizontal align property. Uses set/getHorAlign. */
	PROPERTY(UILabel, bool)           wordWrap;            /** Words wrapping flag property. Uses set/getWordWrap. */
	PROPERTY(UILabel, float)          charactersHeight;    /** Characters height property, pixels. Uses set/getCharactersHeight. */
	PROPERTY(UILabel, float)          charactersDistCoef;  /** Characters distance coef, 1 is standard. Uses set/getCharactersDistCoef. */
	PROPERTY(UILabel, float)          linesDistCoef;       /** Lines distance coef, 1 is standard. Uses set/getLinesDistCoef. */
	
	/** ctor. */
	UILabel(Font* font, const Layout& layout, const String& id = "");

	/** copy-ctor. */
	UILabel(const UILabel& label);

	/** dtor. */
	~UILabel();

	/** Returns copy of widget. */
	UIWidget* Clone() const;		

	/** Sets using font. */
	void SetFont(Font* font);

	/** Returns using font. */
	Font* GetFont() const;

	/** Sets text. */
	void SetText(const WString& text);

	/** Returns text. */
	WString GetText() const;

	/** Sets text. */
	void SetCText(const String& text);

	/** Returns text as string. */
	String GetCText() const;

	/** Return real text size. */
	Vec2F GetTextRealSize();

	/** Sets characters height in pixels. */
	void SetCharactersHeight(const float& height);

	/** Returns characters height in pixels. */
	float GetCharactersHeight() const;

	/** Sets horizontal align. */
	void SetHorAlign(const Font::HorAlign& align);

	/** Returns horizontal align. */
	Font::HorAlign GetHorAlign() const;

	/** Sets vertical align. */
	void SetVerAlign(const Font::VerAlign& align);

	/** returns vertical align. */
	Font::VerAlign GetVerAlign() const;

	/** Sets word wrapping. */
	void SetWordWrap(const bool& flag);

	/** Returns word wrapping. */
	bool GetWordWrap() const;

	/** Sets characters distance coefficient. */
	void SetCharactersDistCoef(const float& coef = 1);

	/** Returns characters distance coef. */
	float GetCharactersDistCoef() const;

	/** Sets lines distance coefficient. */
	void SetLinesDistCoef(const float& coef = 1);

	/** Returns lines distance coefficient. */
	float GetLinesDistCoef() const;

protected:
	/** Drawing current widget. */
	virtual void LocalDraw();

	/** Calls when widget's layout updated. */
	virtual void LayoutUpdated();

	/** Calls when result transparency changed. */
	void TransparencyChanged();
};

CLOSE_O2_NAMESPACE
