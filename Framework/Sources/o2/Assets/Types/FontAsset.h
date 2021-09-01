#pragma once

#include "o2/Assets/Asset.h"
#include "o2/Assets/AssetRef.h"
#include "o2/Render/FontRef.h"

namespace o2
{
	class Font;

	// ----------
	// Font asset
	// ----------
	class FontAsset: public Asset
	{
	public:
		PROPERTIES(FontAsset);
		GETTER(FontRef, font, GetFont); // Font getter

	public:
		// Default constructor
		FontAsset();

		// Default constructor
		FontAsset(AssetMeta* meta);

		// Copy-constructor
		FontAsset(const FontAsset& asset);

		// Check equals operator
		FontAsset& operator=(const FontAsset& asset);

		// Returns font pointer
		virtual FontRef GetFont() const;

		// Returns editor sorting weight
		static int GetEditorSorting() { return 93; }

		SERIALIZABLE(FontAsset);

	protected:
		FontRef mFont;

		friend class Assets;
	};

	typedef Ref<FontAsset> FontAssetRef;
}

CLASS_BASES_META(o2::FontAsset)
{
	BASE_CLASS(o2::Asset);
}
END_META;
CLASS_FIELDS_META(o2::FontAsset)
{
	FIELD().NAME(font).PUBLIC();
	FIELD().NAME(mFont).PROTECTED();
}
END_META;
CLASS_METHODS_META(o2::FontAsset)
{

	PUBLIC_FUNCTION(FontRef, GetFont);
	PUBLIC_STATIC_FUNCTION(int, GetEditorSorting);
}
END_META;
