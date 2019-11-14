#pragma once

#include "ft2build.h"
#include FT_FREETYPE_H

#include "Render/Font.h"
#include "Utils/Property.h"
#include "Utils/Serialization/Serializable.h"
#include "Utils/Tools/RectPacker.h"
#include "Utils/Types/Containers/Map.h"

namespace o2
{
	class RectsPacker;
	class Bitmap;

	// -----------
	// Vector font
	// -----------
	class VectorFont: public Font
	{
	public:
		// ---------------------
		// Font effect interface
		// ---------------------
		class Effect: public ISerializable
		{
		public:
			// Processes glyph bitmap
			virtual void Process(Bitmap* bitmap) {};

			// Returns needs extending size for glyph bitmap
			virtual Vec2I GetSizeExtend() const { return Vec2I(); };

			// Check effect equals
			virtual bool IsEqual(Effect* other) const { return GetType() == other->GetType(); }

			SERIALIZABLE(Effect);
		};

	public:
		// Default constructor
		VectorFont();

		// Constructor with loading file
		VectorFont(const String& fileName);

		// Copy-constructor
		VectorFont(const VectorFont& other);

		// Destructor
		~VectorFont();

		// Loads font
		bool Load(const String& fileName);

		// Returns font file name
		String GetFileName() const;

		// Returns base height in pixels for font with size
		float GetHeightPx(int height) const;

		// Returns line height in pixels for font with size
		float GetLineHeightPx(int height) const;

		// Checks characters for preloading
		void CheckCharacters(const WString& needChararacters, int height);

		// Adds effect
		Effect* AddEffect(Effect* effect);

		// Adds effect
		template<typename _eff_type, typename ... _args>
		_eff_type* AddEffect(_args ... args);

		// Removes effect
		void RemoveEffect(Effect* effect);

		// Removes all effects
		void RemoveAllEffects();

		// Removes all cached characters
		void Reset();

	protected:
		struct PackLine;

		//-------------------------------
		// Font glyph rendering character
		// ------------------------------
		struct CharDef
		{
			PackLine* packLine;
			RectI     rect;
			Character character;
			Bitmap*   bitmap;

			bool operator==(const CharDef& other) const { return false; }
		};
		typedef Vector<CharDef> CharDefsVec;

		// -----------------------
		// Characters packing line
		// -----------------------
		struct PackLine
		{
			int         position = 0;
			int         height = 0;
			int         length = 0;
			CharDefsVec characters;

			bool operator==(const PackLine& other) const { return false; }
		};
		typedef Vector<PackLine*> PackLinesVec;

		typedef Vector<Effect*> EffectsVec;
		typedef Map<int, float> HeightsMap;

	protected:
		String  mFileName;     // Source file name
		FT_Face mFreeTypeFace; // Free Type font face

		EffectsVec mEffects; // Font effects

		PackLinesVec mPackLines;           // Packed symbols lines
		int          mLastPackLinePos = 0; // Last packed line bottom pos

		mutable HeightsMap mHeights; // Cached line heights

	protected:
		// Updates characters set
		void UpdateCharacters(Vector<wchar_t>& newCharacters, int height);

		// Renders new characters
		void RenderNewCharacters(Vector<wchar_t>& newCharacters, int height);

		// Packs character in line 
		void PackCharacter(CharDef& character, int height);
	};

	template<typename _eff_type, typename ... _args>
	_eff_type* VectorFont::AddEffect(_args ... args)
	{
		return (_eff_type*)AddEffect(mnew _eff_type(args ...));
	}
}

CLASS_BASES_META(o2::VectorFont::Effect)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(o2::VectorFont::Effect)
{
}
END_META;
CLASS_METHODS_META(o2::VectorFont::Effect)
{

	PUBLIC_FUNCTION(void, Process, Bitmap*);
	PUBLIC_FUNCTION(Vec2I, GetSizeExtend);
	PUBLIC_FUNCTION(bool, IsEqual, Effect*);
}
END_META;
