#pragma once

#include "Assets/Asset.h"
#include "Render/FontRef.h"

namespace o2
{
	class Font;

	// -----------------
	// Bitmap font asset
	// -----------------
	class BitmapFontAsset: public Asset
	{
	public:
		class MetaInfo;

	public:
		Getter<MetaInfo*> meta; // Meta information getter
		Getter<FontRef>   font; // Font getter

		// Default constructor
		BitmapFontAsset();

		// Constructor by path - loads asset by path
		BitmapFontAsset(const String& path);

		// Constructor by id - loads asset by id
		BitmapFontAsset(UID id);

		// Copy-constructor
		BitmapFontAsset(const BitmapFontAsset& asset);

		// Destructor
		~BitmapFontAsset();

		// Check equals operator
		BitmapFontAsset& operator=(const BitmapFontAsset& asset);

		// Check equals operator
		bool operator==(const BitmapFontAsset& other) const;

		// Check not equals operator
		bool operator!=(const BitmapFontAsset& other) const;

		// Returns meta information
		MetaInfo* GetMeta() const;

		// Returns font pointer
		FontRef GetFont() const;

		// Returns extensions string
		const char* GetFileExtensions() const;

		SERIALIZABLE(BitmapFontAsset);

	public:
		// ----------------
		// Meta information
		// ----------------
		class MetaInfo: public IMetaInfo
		{
		public:
			// Returns asset type id
			Type::Id GetAssetType() const;

			SERIALIZABLE(MetaInfo);
		};

	protected:
		FontRef mFont;

	protected:
		// Loads data
		void LoadData(const String& path);

		// Initializes properties
		void InitializeProperties();
	};
}