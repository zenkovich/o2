#pragma once

#include "o2/Render/Texture.h"
#include "o2/Utils/Basic/IObject.h"

namespace o2
{
	class TextureRef: public IObject
	{
		IOBJECT(TextureRef);

	public:
		// Default constructor
		TextureRef();

		// Constructor
		TextureRef(Texture* texture);

		// Constructor
		TextureRef(const Vec2I& size, 
				   TextureFormat format = TextureFormat::R8G8B8A8,
				   Texture::Usage usage = Texture::Usage::Default);

		// Constructor from file @SCRIPTABLE
		TextureRef(const String& fileName);

		// Constructor from bitmap
		TextureRef(Bitmap* bitmap);

		// Constructor from atlas page
		TextureRef(UID atlasAssetId, int page);

		// Constructor from atlas page
		TextureRef(const String& atlasAssetName, int page);

		// Copy-constructor
		TextureRef(const TextureRef& other);

		// Destructor
		~TextureRef();

		// Assign operator
		TextureRef& operator=(const TextureRef& other);

		// Texture pointer operator
		Texture* operator->();

		// Constant texture pointer operator
		const Texture* operator->() const;

		// Check equal operator
		bool operator==(const TextureRef& other) const;

		// Check not equal operator
		bool operator!=(const TextureRef& other) const;

		// Returns original texture pointer
		Texture* Get() const;

		// Returns true if texture isn't null @SCRIPTABLE
		bool IsValid() const;

		// Returns true if texture isn't null
		operator bool() const;

		static TextureRef Null();

	protected:
		Texture* mTexture;

		friend class Render;
		friend class Texture;
	};

	// Returns reference to null texture
	TextureRef NoTexture();
}
// --- META ---

CLASS_BASES_META(o2::TextureRef)
{
	BASE_CLASS(o2::IObject);
}
END_META;
CLASS_FIELDS_META(o2::TextureRef)
{
	FIELD().PROTECTED().NAME(mTexture);
}
END_META;
CLASS_METHODS_META(o2::TextureRef)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(Texture*);
	FUNCTION().PUBLIC().CONSTRUCTOR(const Vec2I&, TextureFormat, Texture::Usage);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().CONSTRUCTOR(const String&);
	FUNCTION().PUBLIC().CONSTRUCTOR(Bitmap*);
	FUNCTION().PUBLIC().CONSTRUCTOR(UID, int);
	FUNCTION().PUBLIC().CONSTRUCTOR(const String&, int);
	FUNCTION().PUBLIC().CONSTRUCTOR(const TextureRef&);
	FUNCTION().PUBLIC().SIGNATURE(Texture*, Get);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(bool, IsValid);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(TextureRef, Null);
}
END_META;
// --- END META ---
