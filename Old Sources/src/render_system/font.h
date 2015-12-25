#pragma once

#include "public.h"
#include "texture.h"
#include "util/math/rect.h"
#include "util/string.h"

OPEN_O2_NAMESPACE

class Mesh;
class RenderSystem;

/** Font. Containing array of symbol glyphs, symbol index table. */
class Font
{
	friend class FontManager;
	friend class Text;

public:
	enum class VerAlign { Top, Middle, Bottom, Both };
	enum class HorAlign { Left, Middle, Right, Both };

	/** Character definition structure. Containing texture source rect, size, offset, advance and character id. */
	struct Character
	{
		RectF  mTexSrc;  /** texture source rect. */
		Vec2F  mSize;    /** Size of source rect. */
		Vec2F  mOffset;  /** Symbol offset. */
		float  mAdvance; /** Symbol advance. */
		uint16 mCharId;  /** Character id. */
	};

	/** text symbols layouts set. */
	struct TextSymbolsSet
	{
		/** Simple symbol definition structure. */
		struct SymbolDef
		{
			RectF  mFrame;  /** Frame of symbol layout. */
			RectF  mTexSrc; /** Texture source rect. */
			uint16 mCharId; /** Character id. */
			Vec2F  mOffset; /** Character offset. */
			float  mAdvance;/** Character advance. */

			SymbolDef() {}
			SymbolDef(const Vec2F& position, const Vec2F& size, const RectF& texSrc, uint16 charId, const Vec2F& offs, float advance):
				mFrame(position, position + size), mTexSrc(texSrc), mCharId(charId), mOffset(offs), mAdvance(advance) {}
		};
		typedef Array<SymbolDef> SymbolDefsArr;

		/** Line definition structure. */
		struct LineDef
		{
			SymbolDefsArr mSymbols;       /** Symbols in line. */
			WString      mString;        /** Line string. */
			Vec2F        mSize;          /** Size of line in pixels. */
			Vec2F        mPosition;      /** Position of line. */
			int          mLineBegSymbol; /** Index of line beginning symbol. */
			int          mSpacesCount;   /** Spaces count at line. */
			bool         mEndedNewLine;  /** True, if line ended by new line character. */

			LineDef():mLineBegSymbol(0), mSpacesCount(0), mEndedNewLine(false) {}
		};
		typedef Array<LineDef> LineDefsArr;

		Font*       mFont;
		WString     mText;               /** Text string. */
		Vec2F       mPosition;           /** Position, in pixels. */
		Vec2F       mAreaSize;           /** Area size, in pixels. */
		Vec2F       mRealSize;           /** Real text size. */
		HorAlign    mHorAlign;           /** Horizontal align. */
		VerAlign    mVerAlign;           /** Vertical align. */
		bool        mWordWrap;           /** True, when words wrapping. */
		float       mCharactersDistCoef; /** Characters distance coef, 1 is standard. */
		float       mLinesDistCoef;      /** Lines distance coef, 1 is standard. */
		LineDefsArr mLineDefs;           /** Lines definitions. */

		/** Calculating characters layout by parameters. */
		void Initialize(Font* font, const WString& text, const Vec2F& position, const Vec2F& areaSize, HorAlign horAlign,
						VerAlign verAlign, bool wordWrap, float charsDistCoef, float linesDistCoef);
	};

protected:
	enum { nMaxSymbolId = 0xffff };

	String     mName;            /** Font name. */
	Character* mCharacters;      /** Characters array. */
	int        mCharactersCount; /** Characters count in array. */
	bool       mAllSymbolReturn; /** True, when all symbols is new line symbols. */
	float      mLineHeight;      /** Line height, in pixels. */
	float      mBase;            /** Base, in pixels. */
	uint16*    mCharacterIds;    /** Characters ids array. */
	TextureRef mTexture;         /** Texture. */

protected:
	/** ctor. */
	Font();

	/** deprecate copy-ctor. */
	Font(const Font& font);

	/** dtor. */
	~Font();

	/** Create font from config file. */
	void Create(const String& fontFile);

	/** Create font from BMFont config file. */
	void CreateFromBMFont(const String& fontFile);

public:
	/** Returns line height. */
	float GetLineHeight() const;

	/** Returns base height. */
	float GetBase() const;

	/** Returns text size. */
	Vec2F GetTextSize(const WString& text, const Vec2F& areaSize = Vec2F(), HorAlign horAlign = HorAlign::Left,
					  VerAlign verAlign = VerAlign::Top, bool wordWrap = true, float charsDistCoef = 1.0f,
					  float linesDistCoef = 1.0f);

	/** Returns symbol advance. */
	float GetSymbolAdvance(uint16 id);
};

CLOSE_O2_NAMESPACE
