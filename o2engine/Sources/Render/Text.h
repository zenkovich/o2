#pragma once

#include "Render/FontRef.h"
#include "Render/RectDrawable.h"
#include "Utils/String.h"

namespace o2
{
	class Asset;
	class BitmapFontAsset;
	class Mesh;
	class Render;
	class VectorFontAsset;

	// ------------------------------------------------------------------------------------------
	// Text renderer class. Using font, basis and many style parameters. Caching text into meshes
	// ------------------------------------------------------------------------------------------
	class Text: public IRectDrawable
	{
	public:
		struct SymbolsSet;

		enum class VerAlign { Top, Middle, Bottom, Both };
		enum class HorAlign { Left, Middle, Right, Both };
		
	public:
		Property<FontRef>   font;                // Font pointer property
		Property<WString>   text;                // Text property, wstring
		Property<String>    ctext;               // Text property, string
		Property<VerAlign>  verAlign;            // vertical align property
		Property<HorAlign>  horAlign;            // Horizontal align property
		Property<bool>      wordWrap;            // Words wrapping flag property
		Property<float>     symbolsDistanceCoef; // Characters distance coef, 1 is standard
		Property<float>     linesDistanceCoef;   // Lines distance coef, 1 is standard

		// Default constructor
		Text();

		// Constructor
		Text(FontRef font);

		// Constructor
		Text(const String& fontFileName);

		// Constructor
		Text(AssetId fontAssetId);

		// Constructor
		Text(Ptr<BitmapFontAsset> fontAsset);

		// Constructor
		Text(Ptr<VectorFontAsset> fontAsset);

		// Copy-constructor
		Text(const Text& text);

		// Destructor
		~Text();

		// Assign operator
		Text& operator=(const Text& other);

		// Draw text
		void Draw();

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
		static Vec2F GetTextSize(const WString& text, Ptr<Font> font,
								 const Vec2F& areaSize = Vec2F(), 
								 HorAlign horAlign = HorAlign::Left, VerAlign verAlign = VerAlign::Top, 
								 bool wordWrap = true, float charsDistCoef = 1.0f, float linesDistCoef = 1.0f);

		SERIALIZABLE_IMPL(Text);

		IOBJECT(Text)
		{
			BASE_CLASS(IRectDrawable);
			FIELD(font);
			FIELD(text);
			FIELD(ctext);
			FIELD(verAlign);
			FIELD(horAlign);
			FIELD(wordWrap);
			FIELD(symbolsDistanceCoef);
			FIELD(linesDistanceCoef);
			FIELD(mFont);
			FIELD(mMeshes);
			FIELD(mLastTransform);
			FIELD(mSymbolsSet);

			SRLZ_FIELD(mFontAssetId);
			SRLZ_FIELD(mText);
			SRLZ_FIELD(mVerAlign);
			SRLZ_FIELD(mHorAlign);
			SRLZ_FIELD(mWordWrap);
			SRLZ_FIELD(mSymbolsDistCoef);
			SRLZ_FIELD(mLinesDistanceCoef);
		}

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
			typedef Vector<Symbol> SymbolDefsVec;

			// -------------------------
			// Line definition structure
			// -------------------------
			struct Line
			{
				SymbolDefsVec mSymbols;       // Symbols in line
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
			typedef Vector<Line> LineDefsVec;

		public:
			FontRef     mFont;            // Font
			WString     mText;            // Text string
			Vec2F       mPosition;        // Position, in pixels
			Vec2F       mAreaSize;        // Area size, in pixels
			Vec2F       mRealSize;        // Real text size
			HorAlign    mHorAlign;        // Horizontal align
			VerAlign    mVerAlign;        // Vertical align
			bool        mWordWrap;        // True, when words wrapping
			float       mSymbolsDistCoef; // Characters distance coefficient, 1 is standard
			float       mLinesDistCoef;   // Lines distance coefficient, 1 is standard
			LineDefsVec mLines;           // Lines definitions

		public:
			// Calculating characters layout by parameters
			void Initialize(FontRef font, const WString& text, const Vec2F& position, const Vec2F& areaSize, 
							HorAlign horAlign, VerAlign verAlign, bool wordWrap, float charsDistCoef, float linesDistCoef);

			// Moves symbols 
			void Move(const Vec2F& offs);
		};

	protected:
		typedef Vector<Ptr<Mesh>> MeshesVec;

		const UInt mMeshMaxPolyCount = 4096;

		WString    mText;              // Wide char string, containing rendering text
		AssetId    mFontAssetId;       // Font asset id
		FontRef    mFont;              // Using font
		float      mSymbolsDistCoef;   // Characters distance coef, 1 is standard
		float      mLinesDistanceCoef; // Lines distance coef, 1 is standard
		VerAlign   mVerAlign;          // Vertical align
		HorAlign   mHorAlign;          // Horizontal align
		bool       mWordWrap;          // True, when words wrapping
									   
		MeshesVec  mMeshes;            // Meshes vector
		Basis      mLastTransform;     // Last mesh update transformation
									   
		SymbolsSet mSymbolsSet;        // Symbols set definition
									   
		bool       mUpdatingMesh;      // True, when mesh is already updating

	protected:
		// Updating meshes
		void UpdateMesh();

		// Transforming meshes by basis
		void TransformMesh(const Basis& bas);

		// Preparing meshes for characters count
		void PrepareMesh(int charactersCount);

		// Calculates and returns text basis
		Basis CalculateTextBasis() const;

		// Calls when color was changed
		void ColorChanged();

		// Calls when basis was changed
		void BasisChanged();

		// Calling when deserializing
		void OnDeserialized(const DataNode& node);

		// Initializing properties
		void InitializeProperties();
	};
}
