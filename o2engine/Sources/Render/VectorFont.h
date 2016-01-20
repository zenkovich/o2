#pragma once

#include "ft2build.h"
#include FT_FREETYPE_H

#include "Render/Font.h"
#include "Utils/Containers/Dictionary.h"
#include "Utils/Property.h"
#include "Utils/RectPacker.h"
#include "Utils/Serialization.h"

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
		Property<UInt> size; // Characters size property

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

		// Sets font size
		void SetSize(UInt size);

		// Returns font size
		UInt GetSize() const;

		// Returns font file name
		String GetFileName() const;

		// Checks characters for pre loading
		void CheckCharacters(const WString& needChararacters);

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
		//-------------------------------
		// Font glyph rendering character
		// ------------------------------
		struct CharDef
		{
			RectsPacker::Rect* mPackRect;  // Pack rect
			Character          mCharacter; // Character
			Bitmap*            mBitmap;    // Bitmap

			bool operator==(const CharDef& other) const { return false; }
		};
		typedef Vector<CharDef> CharDefsVec;
		typedef Vector<Effect*> EffectsVec;

	protected:

		String        mFileName;     // Source file name

		RectsPacker   mRectsPacker;  // Characters packer
		UInt          mSize;         // Font size
		FT_Face       mFreeTypeFace; // Free Type font face
		EffectsVec    mEffects;      // Font effects

	protected:
		// Updates characters set
		void UpdateCharacters(Vector<wchar_t>& newCharacters);

		// Extracts characters from texture
		void ExtractCharacterDefsFromTexture(CharDefsVec& charDefs);

		// Renders new characters
		void RenderNewCharacters(CharDefsVec& charDefs, Vector<wchar_t>& newCharacters);

		// Pack all characters
		void PackCharactersDefs(CharDefsVec& charDefs);

		// Initializes properties
		void InitializeProperties();
	};

	template<typename _eff_type, typename ... _args>
	_eff_type* VectorFont::AddEffect(_args ... args)
	{
		return (_eff_type*)AddEffect(mnew _eff_type(args ...));
	}
}
