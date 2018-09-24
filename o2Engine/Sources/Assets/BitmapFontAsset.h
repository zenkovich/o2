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
		PROPERTIES(BitmapFontAsset);
		GETTER(MetaInfo*, meta, GetMeta); // Meta information getter
		GETTER(FontRef, font, GetFont);   // Font getter

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
		const char* GetFileExtensions() const override;

		SERIALIZABLE(BitmapFontAsset);

	public:
		// ----------------
		// Meta information
		// ----------------
		class MetaInfo: public IMetaInfo
		{
		public:
			// Returns asset type id
			const Type* GetAssetType() const override;

			SERIALIZABLE(MetaInfo);
		};

	protected:
		FontRef mFont;

	protected:
		// Default constructor
		BitmapFontAsset();

		// Constructor by path - loads asset by path
		BitmapFontAsset(const String& path);

		// Constructor by id - loads asset by id
		BitmapFontAsset(UID id);

		// Copy-constructor
		BitmapFontAsset(const BitmapFontAsset& asset);

		// Loads data
		void LoadData(const String& path) override;

		friend class Assets;
	};

	// ---------------------------
	// Bitmap font Asset reference
	// ---------------------------
	class BitmapFontAssetRef: public AssetRef
	{
	public:
		// Creates BitmapFontAsset and returns reference to it
		static BitmapFontAssetRef CreateAsset();

		// Default constructor, references to null
		BitmapFontAssetRef(): AssetRef() {}

		// Copy-constructor
		BitmapFontAssetRef(const AssetRef& other): AssetRef(other) { CheckType<BitmapFontAsset>(); }

		// Copy-constructor
		BitmapFontAssetRef(const BitmapFontAssetRef& other): AssetRef(other) {}

		// Constructor from asset path
		BitmapFontAssetRef(const String& path): AssetRef(path) {}

		// Constructor from asset id
		BitmapFontAssetRef(UID id): AssetRef(id) {}

		// Destructor
		~BitmapFontAssetRef() {}

		// Boolean cast operator, true means that reference is valid
		operator bool() const { return IsValid(); }

		// Assign operator
		BitmapFontAssetRef& operator=(const BitmapFontAssetRef& other) { AssetRef::operator=(other); return *this; }

		// Getter operator
		BitmapFontAsset& operator*() { return *((BitmapFontAsset*)mAssetPtr); }

		// Constant getter operator
		const BitmapFontAsset& operator*() const { return *((BitmapFontAsset*)mAssetPtr); }

		// Asset members and field operator
		BitmapFontAsset* operator->() { return ((BitmapFontAsset*)mAssetPtr); }

		// Constant asset members and field operator
		const BitmapFontAsset* operator->() const { return ((BitmapFontAsset*)mAssetPtr); }

		// Check equals operator
		bool operator==(const BitmapFontAssetRef& other) const { return AssetRef::operator==(other); }

		// Check not equals operator
		bool operator!=(const BitmapFontAssetRef& other) const { return AssetRef::operator!=(other); }

		// Returns asset type
		const Type& GetAssetType() const override { return TypeOf(BitmapFontAsset); }

		SERIALIZABLE(BitmapFontAssetRef);

	protected:
		// Constructor for Assets manager
		BitmapFontAssetRef(Asset* assetPtr, int* refCounter): AssetRef(assetPtr, refCounter) {}
	};
}

CLASS_BASES_META(o2::BitmapFontAsset)
{
	BASE_CLASS(o2::Asset);
}
END_META;
CLASS_FIELDS_META(o2::BitmapFontAsset)
{
	PUBLIC_FIELD(meta);
	PUBLIC_FIELD(font);
	PROTECTED_FIELD(mFont);
}
END_META;
CLASS_METHODS_META(o2::BitmapFontAsset)
{

	PUBLIC_FUNCTION(MetaInfo*, GetMeta);
	PUBLIC_FUNCTION(FontRef, GetFont);
	PUBLIC_FUNCTION(const char*, GetFileExtensions);
	PROTECTED_FUNCTION(void, LoadData, const String&);
}
END_META;

CLASS_BASES_META(o2::BitmapFontAssetRef)
{
	BASE_CLASS(o2::AssetRef);
}
END_META;
CLASS_FIELDS_META(o2::BitmapFontAssetRef)
{
}
END_META;
CLASS_METHODS_META(o2::BitmapFontAssetRef)
{

	PUBLIC_FUNCTION(const Type&, GetAssetType);
}
END_META;

CLASS_BASES_META(o2::BitmapFontAsset::MetaInfo)
{
	BASE_CLASS(o2::Asset::IMetaInfo);
}
END_META;
CLASS_FIELDS_META(o2::BitmapFontAsset::MetaInfo)
{
}
END_META;
CLASS_METHODS_META(o2::BitmapFontAsset::MetaInfo)
{

	PUBLIC_FUNCTION(const Type*, GetAssetType);
}
END_META;
