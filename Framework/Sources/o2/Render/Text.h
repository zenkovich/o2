#pragma once

#include "o2/Assets/Asset.h"
#include "o2/Assets/AssetTypes/BitmapFontAsset.h"
#include "o2/Assets/AssetTypes/VectorFontAsset.h"
#include "o2/Render/FontRef.h"
#include "o2/Render/RectDrawable.h"
#include "o2/Utils/Types/String.h"

namespace o2
{
	class Mesh;
	class Render;

	// ------------------------------------------------------------------------------------------
	// Text renderer class. Using font, basis and many style parameters. Caching text into meshes
	// ------------------------------------------------------------------------------------------
	class Text: public IRectDrawable
	{
	public:
		struct SymbolsSet;

	public:
		PROPERTIES(Text);
		PROPERTY(FontRef, font, SetFont, GetFont);                                            // Font reference property
		PROPERTY(FontAssetRef, fontAsset, SetFontAsset, GetFontAsset);                        // Font asset reference property
		PROPERTY(WString, text, SetText, GetText);                                            // Text property, wstring
		PROPERTY(int, height, SetHeight, GetHeight);                                          // Text height
		PROPERTY(VerAlign, verAlign, SetVerAlign, GetVerAlign);                               // vertical align property
		PROPERTY(HorAlign, horAlign, SetHorAlign, GetHorAlign);                               // Horizontal align property
		PROPERTY(bool, wordWrap, SetWordWrap, GetWordWrap);                                   // Words wrapping flag property
		PROPERTY(bool, dotsEngings, SetDotsEngings, IsDotsEngings);                           // Dots endings when overflow property
		PROPERTY(float, symbolsDistanceCoef, SetSymbolsDistanceCoef, GetSymbolsDistanceCoef); // Characters distance coef, 1 is standard
		PROPERTY(float, linesDistanceCoef, SetLinesDistanceCoef, GetLinesDistanceCoef);       // Lines distance coef, 1 is standard

	public:
		// Default constructor
		Text();

		// Constructor
		Text(FontRef font);

		// Constructor
		Text(const String& fontFileName);

		// Constructor
		Text(const UID& fontAssetId);

		// Constructor
		Text(const BitmapFontAssetRef& fontAsset);

		// Constructor
		Text(const VectorFontAssetRef& fontAsset);

		// Copy-constructor
		Text(const Text& text);

		// Destructor
		~Text();

		// Assign operator
		Text& operator=(const Text& other);

		// Draw text
		void Draw() override;

		// Sets using font
		void SetFont(FontRef font);

		// Returns using font
		FontRef GetFont() const;

		// Sets bitmap font asset 
		void SetFontAsset(const FontAssetRef& asset);

		// Returns asset by font asset id
		FontAssetRef GetFontAsset() const;

		// Sets font height
		void SetHeight(int height);

		// Returns font height
		int GetHeight() const;

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

		// Sets word wrapping
		void SetWordWrap(bool flag);

		// Returns word wrapping
		bool GetWordWrap() const;

		// Sets dots endings
		void SetDotsEngings(bool flag);

		// Returns dots endings
		bool IsDotsEngings() const;

		// Sets characters distance coefficient
		void SetSymbolsDistanceCoef(float coef = 1);

		// Returns characters distance coef
		float GetSymbolsDistanceCoef() const;

		// Sets lines distance coefficient
		void SetLinesDistanceCoef(float coef = 1);

		// Returns lines distance coefficient
		float GetLinesDistanceCoef() const;

		// Returns symbol set structure pointer
		SymbolsSet& GetSymbolsSet();

		// Returns real text size
		Vec2F GetRealSize();

		// Returns real text rectangle
		RectF GetRealRect();

		// Returns text size
		static Vec2F GetTextSize(const WString& text, Font* font, int height = 11,
								 const Vec2F& areaSize = Vec2F(),
								 HorAlign horAlign = HorAlign::Left, VerAlign verAlign = VerAlign::Top,
								 bool wordWrap = true, bool dotsEngings = false, float charsDistCoef = 1.0f,
								 float linesDistCoef = 1.0f);

		SERIALIZABLE(Text);

	public:
		// ----------------
		// Text symbols set
		// ----------------
		struct SymbolsSet
		{
			// ----------------------------------
			// Simple symbol definition structure
			// ----------------------------------
			struct Symbol
			{
				RectF  mFrame;   // Frame of symbol layout
				RectF  mTexSrc;  // Texture source rect
				UInt16 mCharId;  // Character id
				Vec2F  mOrigin;  // Character offset
				float  mAdvance; // Character advance

			public:
				// Default constructor
				Symbol();

				// Constructor
				Symbol(const Vec2F& position, const Vec2F& size, const RectF& texSrc, UInt16 charId,
					   const Vec2F& origin, float advance);

			 // Equals operator
				bool operator==(const Symbol& other) const;
			};

			// -------------------------
			// Line definition structure
			// -------------------------
			struct Line
			{
				Vector<Symbol> mSymbols;       // Symbols in line
				WString       mString;        // Line string
				Vec2F         mSize;          // Size of line in pixels
				Vec2F         mPosition;      // Position of line
				int           mLineBegSymbol; // Index of line beginning symbol
				int           mSpacesCount;   // Spaces count at line
				bool          mEndedNewLine;  // True, if line ended by new line character

			public:
				// Default constructor
				Line();

				// Equals operator
				bool operator==(const Line& other) const;
			};

		public:
			FontRef  mFont;            // Font
			int      mHeight;          // Text height
			WString  mText;            // Text string
			Vec2F    mPosition;        // Position, in pixels
			Vec2F    mAreaSize;        // Area size, in pixels
			Vec2F    mRealSize;        // Real text size
			HorAlign mHorAlign;        // Horizontal align
			VerAlign mVerAlign;        // Vertical align
			bool     mWordWrap;        // True, when words wrapping
			bool     mDotsEndings;     // Dots ending when overflow
			float    mSymbolsDistCoef; // Characters distance coefficient, 1 is standard
			float    mLinesDistCoef;   // Lines distance coefficient, 1 is standard

			Vector<Line> mLines; // Lines definitions

		public:
			// Calculating characters layout by parameters
			void Initialize(FontRef font, const WString& text, int height, const Vec2F& position, const Vec2F& areaSize,
							HorAlign horAlign, VerAlign verAlign, bool wordWrap, bool dotsEngings, float charsDistCoef,
							float linesDistCoef);

			// Moves symbols 
			void Move(const Vec2F& offs);
		};

	protected:
		static const char* mBasicSymbolsPreset;
		const UInt mMeshMaxPolyCount = 4096;

		WString  mText;              // Wide char string, containing rendering text @SERIALIZABLE
		UID      mFontAssetId;       // Font asset id @SERIALIZABLE
		FontRef  mFont;              // Using font
		int      mHeight;            // Text height @SERIALIZABLE
		float    mSymbolsDistCoef;   // Characters distance coef, 1 is standard @SERIALIZABLE
		float    mLinesDistanceCoef; // Lines distance coef, 1 is standard @SERIALIZABLE
		VerAlign mVerAlign;          // Vertical align @SERIALIZABLE
		HorAlign mHorAlign;          // Horizontal align @SERIALIZABLE
		bool     mWordWrap;          // True, when words wrapping @SERIALIZABLE
		bool     mDotsEndings;       // If true, text will end on '...' @SERIALIZABLE

		Vector<Mesh*> mMeshes;        // Meshes vector
		Basis         mLastTransform; // Last mesh update transformation

		SymbolsSet mSymbolsSet; // Symbols set definition

		bool mUpdatingMesh; // True, when mesh is already updating

	protected:
		// Updating meshes
		void UpdateMesh();

		// Transforming meshes by basis
		void TransformMesh(const Basis& bas);

		// Preparing meshes for characters count
		void PrepareMesh(int charactersCount);

		// Calculates and returns text basis
		Basis CalculateTextBasis() const;

		// It is called when color was changed
		void ColorChanged() override;

		// It is called when basis was changed
		void BasisChanged() override;

		// Calling when deserializing
		void OnDeserialized(const DataNode& node) override;
	};
}

CLASS_BASES_META(o2::Text)
{
	BASE_CLASS(o2::IRectDrawable);
}
END_META;
CLASS_FIELDS_META(o2::Text)
{
	PUBLIC_FIELD(font);
	PUBLIC_FIELD(fontAsset);
	PUBLIC_FIELD(text);
	PUBLIC_FIELD(height);
	PUBLIC_FIELD(verAlign);
	PUBLIC_FIELD(horAlign);
	PUBLIC_FIELD(wordWrap);
	PUBLIC_FIELD(dotsEngings);
	PUBLIC_FIELD(symbolsDistanceCoef);
	PUBLIC_FIELD(linesDistanceCoef);
	PROTECTED_FIELD(mMeshMaxPolyCount);
	PROTECTED_FIELD(mText).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mFontAssetId).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mFont);
	PROTECTED_FIELD(mHeight).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mSymbolsDistCoef).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mLinesDistanceCoef).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mVerAlign).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mHorAlign).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mWordWrap).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mDotsEndings).SERIALIZABLE_ATTRIBUTE();
	PROTECTED_FIELD(mMeshes);
	PROTECTED_FIELD(mLastTransform);
	PROTECTED_FIELD(mSymbolsSet);
	PROTECTED_FIELD(mUpdatingMesh);
}
END_META;
CLASS_METHODS_META(o2::Text)
{

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, SetFont, FontRef);
	PUBLIC_FUNCTION(FontRef, GetFont);
	PUBLIC_FUNCTION(void, SetFontAsset, const FontAssetRef&);
	PUBLIC_FUNCTION(FontAssetRef, GetFontAsset);
	PUBLIC_FUNCTION(void, SetHeight, int);
	PUBLIC_FUNCTION(int, GetHeight);
	PUBLIC_FUNCTION(void, SetText, const WString&);
	PUBLIC_FUNCTION(WString, GetText);
	PUBLIC_FUNCTION(void, SetHorAlign, HorAlign);
	PUBLIC_FUNCTION(HorAlign, GetHorAlign);
	PUBLIC_FUNCTION(void, SetVerAlign, VerAlign);
	PUBLIC_FUNCTION(VerAlign, GetVerAlign);
	PUBLIC_FUNCTION(void, SetWordWrap, bool);
	PUBLIC_FUNCTION(bool, GetWordWrap);
	PUBLIC_FUNCTION(void, SetDotsEngings, bool);
	PUBLIC_FUNCTION(bool, IsDotsEngings);
	PUBLIC_FUNCTION(void, SetSymbolsDistanceCoef, float);
	PUBLIC_FUNCTION(float, GetSymbolsDistanceCoef);
	PUBLIC_FUNCTION(void, SetLinesDistanceCoef, float);
	PUBLIC_FUNCTION(float, GetLinesDistanceCoef);
	PUBLIC_FUNCTION(SymbolsSet&, GetSymbolsSet);
	PUBLIC_FUNCTION(Vec2F, GetRealSize);
	PUBLIC_FUNCTION(RectF, GetRealRect);
	PUBLIC_STATIC_FUNCTION(Vec2F, GetTextSize, const WString&, Font*, int, const Vec2F&, HorAlign, VerAlign, bool, bool, float, float);
	PROTECTED_FUNCTION(void, UpdateMesh);
	PROTECTED_FUNCTION(void, TransformMesh, const Basis&);
	PROTECTED_FUNCTION(void, PrepareMesh, int);
	PROTECTED_FUNCTION(Basis, CalculateTextBasis);
	PROTECTED_FUNCTION(void, ColorChanged);
	PROTECTED_FUNCTION(void, BasisChanged);
	PROTECTED_FUNCTION(void, OnDeserialized, const DataNode&);
}
END_META;
