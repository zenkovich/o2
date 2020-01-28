#pragma once

#include "o2/Assets/Asset.h"
#include "o2/Render/FontRef.h"

namespace o2
{
	class Font;

	// ----------
	// Font asset
	// ----------
	class FontAsset : public TAsset<FontAsset>
	{
	public:
		PROPERTIES(FontAsset);
		GETTER(FontRef, font, GetFont);   // Font getter

	public:
		// Check equals operator
		FontAsset& operator=(const FontAsset& asset);

		// Returns font pointer
		virtual FontRef GetFont() const;

		// Returns extensions string
		const char* GetFileExtensions() const override;

		SERIALIZABLE(FontAsset);

	protected:
		FontRef mFont;

	protected:
		// Default constructor
		FontAsset();

		// Copy-constructor
		FontAsset(const FontAsset& asset);

		friend class Assets;
	};

	typedef Ref<FontAsset> FontAssetRef;
}

CLASS_BASES_META(o2::FontAsset)
{
	BASE_CLASS(o2::TAsset<FontAsset>);
}
END_META;
CLASS_FIELDS_META(o2::FontAsset)
{
	PUBLIC_FIELD(font);
	PROTECTED_FIELD(mFont);
}
END_META;
CLASS_METHODS_META(o2::FontAsset)
{

	PUBLIC_FUNCTION(FontRef, GetFont);
	PUBLIC_FUNCTION(const char*, GetFileExtensions);
}
END_META;
