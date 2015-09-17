#pragma once

#include "Render/TextureRef.h"
#include "Utils/CommonTypes.h"
#include "Utils/Containers/Vector.h"
#include "Utils/Math/Rect.h"
#include "Utils/Math/Vector2.h"
#include "Utils/String.h"

namespace o2
{
	class Mesh;
	class Render;

	// -----------------------------------------------------------
	// Font. Containing array of symbol glyphs, symbol index table
	// -----------------------------------------------------------
	class Font
	{
	public:
		// Default constructor
		Font();

		// Copy-constructor
		Font(const Font& font);

		// Destructor
		~Font();

		// Create font from config file
		void Create(const String& fontFile);

		// Create font from BMFont config file
		void CreateFromBMFont(const String& fontFile);

		// Returns line height
		float GetLineHeight() const;

		// Returns base height
		float GetBase() const;

		// Returns symbol advance
		float GetSymbolAdvance(UInt16 id);

	protected:
		enum { nMaxSymbolId = 0xffff };

		// --------------------
		// Character definition
		// --------------------
		struct Character
		{
			RectF  mTexSrc;  // texture source rect
			Vec2F  mSize;    // Size of source rect
			Vec2F  mOffset;  // Symbol offset
			float  mAdvance; // Symbol advance
			UInt16 mCharId;  // Character id

			bool operator==(const Character& other) const;
		};

	protected:
		String     mName;            // Font name
		Character* mCharacters;      // Characters array
		int        mCharactersCount; // Characters count in array
		bool       mAllSymbolReturn; // True, when all symbols is new line symbols
		float      mLineHeight;      // Line height, in pixels
		float      mBase;            // Base, in pixels
		UInt16*    mCharacterIds;    // Characters ids array
		TextureRef mTexture;         // Texture
		RectI      mTextureSrcRect;  // Texture source rectangle

		friend class Text;
	};
}