#pragma once

#include "ft2build.h"
#include FT_FREETYPE_H

#include "o2/Render/Font.h"
#include "o2/Utils/Property.h"
#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/Tools/RectPacker.h"
#include "o2/Utils/Types/Containers/Map.h"
#include "o2/Utils/Types/Ref.h"

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
		class Effect: public ISerializable, public RefCounterable
		{
		public:
			// Processes glyph bitmap
			virtual void Process(const Ref<Bitmap>& bitmap) {};

			// Returns needs extending size for glyph bitmap
			virtual Vec2I GetSizeExtend() const;;

			// Check effect equals
			virtual bool IsEqual(const Ref<Effect>& other) const;

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
		const Ref<Effect>& AddEffect(const Ref<Effect>& effect);

		// Adds effect
		template<typename _eff_type, typename ... _args>
		const Ref<_eff_type>& AddEffect(_args ... args);

		// Removes effect
		void RemoveEffect(const Ref<Effect>& effect);

		// Removes all effects
		void RemoveAllEffects();

		// Sets effects list
		void SetEffects(const Vector<Ref<Effect>>& effects);

		// Returns effects list
		const Vector<Ref<Effect>>& GetEffects() const;

		// Removes all cached characters
		void Reset();

	protected:
		struct PackLine;

		//-------------------------------
		// Font glyph rendering character
		// ------------------------------
		struct CharDef
		{
			PackLine* packLine;    // Packed line
			RectI       rect;      // Glyph rect
			Character   character; // Glyph character
			Ref<Bitmap> bitmap;    // Glyph bitmap

		public:
			bool operator==(const CharDef& other) const { return false; }
		};

		// -----------------------
		// Characters packing line
		// -----------------------
		struct PackLine: public RefCounterable
		{
			int position = 0; // Line position
			int height = 0;   // Line height
			int length = 0;   // Line length

			Vector<CharDef> characters; // Line characters

		public:
			// Check equals
			bool operator==(const PackLine& other) const { return false; }
		};

	protected:
		String  mFileName;     // Source file name
		FT_Face mFreeTypeFace; // Free Type font face

		Vector<Ref<Effect>> mEffects; // Font effects

		Vector<Ref<PackLine>> mPackLines;           // Packed symbols lines
		int                   mLastPackLinePos = 0; // Last packed line bottom pos

		mutable Map<int, float> mHeights; // Cached line heights

	protected:
		// Updates characters set
		void UpdateCharacters(Vector<wchar_t>& newCharacters, int height);

		// Renders new characters
		void RenderNewCharacters(Vector<wchar_t>& newCharacters, int height);

		// Packs character in line 
		void PackCharacter(CharDef& character, int height);
	};

	template<typename _eff_type, typename ... _args>
	const Ref<_eff_type>& VectorFont::AddEffect(_args ... args)
	{
		return DynamicCast<_eff_type>(AddEffect(mmake<_eff_type>(args ...)));
	}
}

CLASS_BASES_META(o2::VectorFont::Effect)
{
	BASE_CLASS(o2::ISerializable);
	BASE_CLASS(o2::RefCounterable);
}
END_META;
CLASS_FIELDS_META(o2::VectorFont::Effect)
{
}
END_META;
CLASS_METHODS_META(o2::VectorFont::Effect)
{

	FUNCTION().PUBLIC().SIGNATURE(void, Process, const Ref<Bitmap>&);
	FUNCTION().PUBLIC().SIGNATURE(Vec2I, GetSizeExtend);
	FUNCTION().PUBLIC().SIGNATURE(bool, IsEqual, const Ref<Effect>&);
}
END_META;
