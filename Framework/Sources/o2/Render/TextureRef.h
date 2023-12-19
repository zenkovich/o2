#pragma once

#include "o2/Render/Texture.h"
#include "o2/Utils/Basic/IObject.h"
#include "o2/Utils/Types/Ref.h"

namespace o2
{
	template<>
	class Ref<Texture> : public IObject, public BaseRef<Texture>
	{
		IOBJECT(Ref<Texture>);

	public:
		// Implement base ref interface
		BASE_REF_IMPLEMETATION(Texture);

		// Constructor with size and format
		Ref<Texture>(const Vec2I& size,
					 TextureFormat format = TextureFormat::R8G8B8A8,
					 Texture::Usage usage = Texture::Usage::Default);

		// Constructor from file @SCRIPTABLE
		explicit Ref<Texture>(const String& fileName);

		// Constructor from bitmap
		explicit Ref<Texture>(const Bitmap& bitmap);

		// Constructor from atlas page
		Ref<Texture>(UID atlasAssetId, int page);

		// Constructor from atlas page
		Ref<Texture>(const String& atlasAssetName, int page);

		// Returns empty texture
		static Ref<Texture> Null();

	protected:
		friend class Render;
		friend class Texture;
	};
}
// --- META ---

CLASS_BASES_META(o2::Ref<o2::Texture>)
{
    BASE_CLASS(o2::IObject);
    BASE_CLASS(o2::BaseRef<Texture>);
}
END_META;
CLASS_FIELDS_META(o2::Ref<o2::Texture>)
{
}
END_META;
CLASS_METHODS_META(o2::Ref<o2::Texture>)
{

    FUNCTION().PUBLIC().CONSTRUCTOR(const Vec2I&, TextureFormat, Texture::Usage);
    FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().CONSTRUCTOR(const String&);
    FUNCTION().PUBLIC().CONSTRUCTOR(const Bitmap&);
    FUNCTION().PUBLIC().CONSTRUCTOR(UID, int);
    FUNCTION().PUBLIC().CONSTRUCTOR(const String&, int);
    FUNCTION().PUBLIC().SIGNATURE_STATIC(Ref<Texture>, Null);
}
END_META;
// --- END META ---
