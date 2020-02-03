#pragma once

#include "o2/Assets/Types/FontAsset.h"

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

		ASSET_TYPE(BitmapFontAsset, Meta);

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
