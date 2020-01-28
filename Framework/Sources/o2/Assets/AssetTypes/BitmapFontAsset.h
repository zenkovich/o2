#pragma once

#include "o2/Assets/AssetTypes/FontAsset.h"

namespace o2
{
	class Font;

	// -----------------
	// Bitmap font asset
	// -----------------
	class BitmapFontAsset: public FontAsset
	{
	public:
		class Meta;

	public:
		PROPERTIES(BitmapFontAsset);
		GETTER(Meta*, meta, GetMeta); // Meta information getter

	public:
		// Check equals operator
		BitmapFontAsset& operator=(const BitmapFontAsset& asset);

		// Returns meta information
		Meta* GetMeta() const;

		// Returns extensions string
		const char* GetFileExtensions() const override;

		SERIALIZABLE(BitmapFontAsset);

	public:
		// ----------------
		// Meta information
		// ----------------
		class Meta: public TAssetMeta<BitmapFontAsset>
		{
		public:
			SERIALIZABLE(Meta);
		};

	protected:
		// Default constructor
		BitmapFontAsset();

		// Copy-constructor
		BitmapFontAsset(const BitmapFontAsset& asset);

		// Loads data
		void LoadData(const String& path) override;

		friend class Assets;
	};

	typedef Ref<BitmapFontAsset> BitmapFontAssetRef;
}

CLASS_BASES_META(o2::BitmapFontAsset)
{
	BASE_CLASS(o2::FontAsset);
}
END_META;
CLASS_FIELDS_META(o2::BitmapFontAsset)
{
	PUBLIC_FIELD(meta);
}
END_META;
CLASS_METHODS_META(o2::BitmapFontAsset)
{

	PUBLIC_FUNCTION(Meta*, GetMeta);
	PUBLIC_FUNCTION(const char*, GetFileExtensions);
	PROTECTED_FUNCTION(void, LoadData, const String&);
}
END_META;

CLASS_BASES_META(o2::BitmapFontAsset::Meta)
{
	BASE_CLASS(o2::TAssetMeta<BitmapFontAsset>);
}
END_META;
CLASS_FIELDS_META(o2::BitmapFontAsset::Meta)
{
}
END_META;
CLASS_METHODS_META(o2::BitmapFontAsset::Meta)
{
}
END_META;
