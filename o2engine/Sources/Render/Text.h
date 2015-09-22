#pragma once

#include "Render/Font.h"
#include "Render/RectDrawable.h"
#include "Utils/String.h"

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

		enum class VerAlign { Top, Middle, Bottom, Both };
		enum class HorAlign { Left, Middle, Right, Both };
		
	public:
		Property<Ptr<Font>> font;            // Font pointer property
		Property<WString>   text;            // Text property, wstring
		Property<String>    ctext;           // Text property, string
		Property<VerAlign>  verAlign;        // vertical align property
		Property<HorAlign>  horAlign;        // Horizontal align property
		Property<bool>      wordWrap;        // Words wrapping flag property
		Property<float>     symbolsDistCoef; // Characters distance coef, 1 is standard
		Property<float>     linesDistance;   // Lines distance coef, 1 is standard

		// Default constructor
		Text();

		// Constructor
		Text(Ptr<Font> font);

		// Copy-constructor
		Text(const Text& text);

		// Destructor
		~Text();

		// Returns copy of drawable
		Text* Clone() const;

		// Draw text
		void Draw();

		// Sets using font
		void SetFont(Ptr<Font> font);

		// Returns using font
		Ptr<Font> GetFont() const;

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
		void SetSymbolsDistCoef(float coef = 1);

		// Returns characters distance coef
		float GetSymbolsDistCoef() const;

		// Sets lines distance coefficient
		void SetLinesDistance(float distance);

		// Returns lines distance coefficient
		float GetLinesDistance() const;

		// Returns symbol set structure pointer
		SymbolsSet& GetSymbolsSet();

		// Returns real text size
		Vec2F GetRealSize();

		// Returns text size
		static Vec2F GetTextSize(const WString& text, Ptr<Font> font,
								 const Vec2F& areaSize = Vec2F(), 
								 HorAlign horAlign = HorAlign::Left, VerAlign verAlign = VerAlign::Top, 
								 bool wordWrap = true, float charsDistCoef = 1.0f, float linesDistCoef = 1.0f);

	public:
		// ----------------
		// Text symbols set
		// ----------------
		struct SymbolsSet
		{
			// ----------------------------------
			// Simple symbol definition structure
			// ----------------------------------
			struct SymbolDef
			{
				RectF  mFrame;   // Frame of symbol layout
				RectF  mTexSrc;  // Texture source rect
				UInt16 mCharId;  // Character id
				Vec2F  mOffset;  // Character offset
				float  mAdvance; // Character advance

			public:
				// Default constructor
				SymbolDef();

				// Constructor
				SymbolDef(const Vec2F& position, const Vec2F& size, const RectF& texSrc, UInt16 charId,
						  const Vec2F& offs, float advance);

				// Equals operator
				bool operator==(const SymbolDef& other) const;
			};
			typedef Vector<SymbolDef> SymbolDefsVec;

			// -------------------------
			// Line definition structure
			// -------------------------
			struct LineDef
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
				LineDef();

				// Equals operator
				bool operator==(const LineDef& other) const;
			};
			typedef Vector<LineDef> LineDefsVec;

		public:
			Ptr<Font>   mFont;            // Font
			WString     mText;            // Text string
			Vec2F       mPosition;        // Position, in pixels
			Vec2F       mAreaSize;        // Area size, in pixels
			Vec2F       mRealSize;        // Real text size
			HorAlign    mHorAlign;        // Horizontal align
			VerAlign    mVerAlign;        // Vertical align
			bool        mWordWrap;        // True, when words wrapping
			float       mSymbolsDistCoef; // Characters distance coefficient, 1 is standard
			float       mLinesDistance;   // Lines distance coefficient, 1 is standard
			LineDefsVec mLineDefs;        // Lines definitions

		public:
			// Calculating characters layout by parameters
			void Initialize(Ptr<Font> font, const WString& text, const Vec2F& position, const Vec2F& areaSize, HorAlign horAlign,
							VerAlign verAlign, bool wordWrap, float charsDistCoef, float linesDistance);
		};

	protected:
		typedef Vector<Ptr<Mesh>> MeshesVec;

		const UInt mMeshMaxPolyCount = 4096;

		WString    mText;            // Wide char string, containing rendering text
		Ptr<Font>  mFont;            // Using font
		float      mSymbolsDistCoef; // Characters distance coef, 1 is standard
		float      mLinesDistance;   // Lines distance coef, 1 is standard
		VerAlign   mVerAlign;        // Vertical align
		HorAlign   mHorAlign;        // Horizontal align
		bool       mWordWrap;        // True, when words wrapping

		MeshesVec  mMeshes;          // Meshes vector
		Basis      mLastTransform;   // Last mesh update transformation

		SymbolsSet mSymbolsSet;      // Symbols set definition

	protected:
		// Initializing properties
		void InitializeProperties();

		// Updating meshes
		void UpdateMesh();

		// Transforming meshes by basis
		void TransformMesh(const Basis& bas);

		// Preparing meshes for characters count
		void PrepareMesh(int charactersCount);

		// Calculates and returns text basis
		Basis CalculateTextBasis() const;

		// Calls when basis was changed
		void BasisChanged();
	};
}
